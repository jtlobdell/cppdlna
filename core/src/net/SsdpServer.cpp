#include <cppdlna/net/SsdpServer.hpp>
#include <cppdlna/config/Configuration.hpp>
#include <queue>
#include <boost/thread.hpp> // boost threads are interruptible, std threads aren't
#include <boost/chrono.hpp> // required by boost threads
#include <string_view>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>

namespace asio = boost::asio;
namespace ip = boost::asio::ip;
using udp = boost::asio::ip::udp;
namespace http = boost::beast::http;
using udp_stream = boost::beast::basic_stream<udp>;


namespace net {

namespace {

struct search_request {
    std::string_view host;
    std::string_view man;
    std::string_view st;
    std::string_view mx;
    std::string_view cpfn;
    std::string_view user_agent;
    std::string_view tcp_port;
    std::string_view cpuuid;
};

} // anonymous namespace

SsdpServer::SsdpServer(boost::asio::io_context& io_context)
    : listener_socket(io_context,
                      udp::endpoint
                      (
                          ip::make_address(config::get("ssdp.search.interface")),
                          std::stoul(config::get("ssdp.search.port"))
                      )),
      advertiser_stream(io_context,
                        udp::endpoint
                        (
                            ip::make_address(config::get("ssdp.advertisement.address")),
                            std::stoul(config::get("ssdp.advertisement.port"))
                        ))
{
}

void SsdpServer::run()
{
    advertiser_thread = boost::thread(boost::bind(&SsdpServer::startAdvertise, this));
    startReceive(); // handle searches
}

void SsdpServer::stop()
{
    advertiser_thread.interrupt();
}

void SsdpServer::startAdvertise()
{
    // see 1.2 Advertisement
    // multicast discovery messages to 239.255.255.250:1900
    // each discovery message contains 4 major components:
    // a) a notification type sent in an NT (notification type) header field
    // b) a composite identifier for the advertisement, sent in a USN (unique service name) header field
    // c) a url for more info about the device, sent in a LOCATION header field
    // d) a duration for which the advertisement is valid, sent in a CACHE-CONTROL header field
    // a number of discovery messages are sent:
    // - three discovery messages for the root device
    // - two discovery messages for each embedded device
    // - one for each service type in each device

    http::request<http::string_body> root1, root2, root3, emb1, emb2, svc;
    std::string exp_str = config::get("ssdp.advertisement.age");
    unsigned int exp_uint = std::stoul(exp_str);

    root1.clear();
    root1.method(http::verb::notify);
    root1.target("*");
    // note -- newly constructed headers use HTTP/1.1 by default, so we don't need to set it here.
    root1.set("CACHE-CONTROL", "max-age=" + exp_str);
    root1.set("LOCATION", "[url for upnp description for root device]");
    root1.set("NT", "upnp:rootdevice");
    root1.set("NTS", "ssdp:alive");
    root1.set("SERVER", "[OS/version] UPnP/2.0 [product/version]");
    root1.set("USN", "uuid:[device-UUID]::upnp:rootdevice");
    root1.set("BOOTID.UPNP.ORG", "[number increased each time device sends an initial announce or an update message]");
    root1.set("CONFIGID.UPNP.ORG", "[number used for caching description information]");
    root1.set("SEARCHPORT.UPNP.ORG", "[number identifies port on which device responds to unicast M-SEARCH]");

    root2 = root1;
    root2.set("NT", "uuid:[device-UUID]");
    root2.set("USN", "uuid:[device-UUID] (for root device UUID)");

    root3 = root1;
    root3.set("NT", "urn:schemas-upnp-org:device:[deviceType]:[ver]");
    root3.set("USN", "uuid:[device-UUID]::urn:schemas-upnp-org:device:[deviceType]:[ver]");

    emb1 = root1;
    emb1.set("NT", "uuid:[device-UUID]");
    emb1.set("USN", "uuid:[device-UUID]");

    emb2 = root1;
    emb2.set("NT", "urn:schemas-upnp-org:device:[deviceType]:[ver]");
    emb2.set("USN", "uuid:[device-UUID]::urn:schemas-upnp-org:device:[deviceType]:[ver]");

    svc = root1;
    svc.set("NT", "urn:schemas-upnp-org:service:[serviceType]:[ver]");
    svc.set("USN", "uuid:[device-UUID]::urn:schemas-upnp-org:service:[serviceType]:[ver]");

    // initial announcements
    
    // devices should wait a random interval (between 0 and 100ms) before
    // sending an initial set of advertisements in order to reduce the
    // likelihood of network storms; this random interval should also be
    // applied on occasions where the device obtains a new IP address or
    // a new UPnP-enabled interface is installed

    // sleep(random(0, 100ms));
    http::write(advertiser_stream, root1); // delay in between these?
    http::write(advertiser_stream, root2);
    http::write(advertiser_stream, root3);
    http::write(advertiser_stream, emb1);
    http::write(advertiser_stream, emb2);
    http::write(advertiser_stream, svc);

    // due to the unreliable nature of udp devices should send the entire
    // set of discovery messages more than once with some delay between
    // the sets e.g. a few hundred ms. to avoid congestion they shouldn't
    // be send more than three times.
    
    unsigned int initial_discoveries = std::stoul(config::get("ssdp.advertisement.initial_discoveries"));
    unsigned int initial_discoveries_spacing = std::stoul(config::get("ssdp.advertisement.initial_discoveries_spacing_ms"));
    for (unsigned int i = 1; i < initial_discoveries; ++i) {
        boost::this_thread::sleep_for(boost::chrono::milliseconds(initial_discoveries_spacing));
        http::write(advertiser_stream, root1); // delay in between these?
        http::write(advertiser_stream, root2);
        http::write(advertiser_stream, root3);
        http::write(advertiser_stream, emb1);
        http::write(advertiser_stream, emb2);
        http::write(advertiser_stream, svc);
    }
    
    // in addition, device shall re-send advertisements periodically
    // prior to expiration. it is recommended that such refreshing of the
    // advertisements be done at a randomly-distributed interval of less
    // than one-half of the advertisement expiration time, so as to provide
    // the opportunity for recovery from lost advertisements before the
    // advertisement expires, and to distribute over time the advertisement
    // of multiple devices on the network in order to avoid spikes in
    // network traffic

    // create a queue of messages and when to re-send advertisements
    std::queue<std::pair<http::request<http::string_body>, boost::chrono::system_clock::time_point>> ad_q;
    auto spacing = boost::chrono::seconds(exp_uint) / 12; // half the expiration time divided by 6 ads
    auto now = boost::chrono::system_clock::now();
    ad_q.push(std::make_pair(root1, now+spacing*1));
    ad_q.push(std::make_pair(root2, now+spacing*2));
    ad_q.push(std::make_pair(root3, now+spacing*3));
    ad_q.push(std::make_pair(emb1, now+spacing*4));
    ad_q.push(std::make_pair(emb2, now+spacing*5));
    ad_q.push(std::make_pair(svc, now+spacing*6));

    // re-send advertisements until terminated (via boost::thread::interrupt)
    try {
        while (true) {
            auto p = ad_q.front();
            ad_q.pop();
            auto h = p.first;
            auto t = p.second;
            boost::this_thread::sleep_until(t);
            http::write(advertiser_stream, h);
            auto next_time = boost::chrono::system_clock::now() + boost::chrono::seconds(exp_uint/2);
            ad_q.push(std::make_pair(h, next_time));
        }
    } catch (boost::thread_interrupted&) {
    }

    // now the server is killed. notify with ssdp:byebye
    
    root1.set("NTS", "ssdp:byebye");
    root2.set("NTS", "ssdp:byebye");
    root3.set("NTS", "ssdp:byebye");
    emb1.set("NTS", "ssdp:byebye");
    emb2.set("NTS", "ssdp:byebye");
    svc.set("NTS", "ssdp:byebye");

    root1.erase("CACHE-CONTROL");
    root1.erase("LOCATION");
    root1.erase("SERVER");
    root1.erase("SEARCHPORT.UPNP.ORG");
    
    root2.erase("CACHE-CONTROL");
    root2.erase("LOCATION");
    root2.erase("SERVER");
    root2.erase("SEARCHPORT.UPNP.ORG");
    
    root3.erase("CACHE-CONTROL");
    root3.erase("LOCATION");
    root3.erase("SERVER");
    root3.erase("SEARCHPORT.UPNP.ORG");

    emb1.erase("CACHE-CONTROL");
    emb1.erase("LOCATION");
    emb1.erase("SERVER");
    emb1.erase("SEARCHPORT.UPNP.ORG");
    
    emb2.erase("CACHE-CONTROL");
    emb2.erase("LOCATION");
    emb2.erase("SERVER");
    emb2.erase("SEARCHPORT.UPNP.ORG");

    svc.erase("CACHE-CONTROL");
    svc.erase("LOCATION");
    svc.erase("SERVER");
    svc.erase("SEARCHPORT.UPNP.ORG");

    http::write(advertiser_stream, root1); // delay in between these?
    http::write(advertiser_stream, root2);
    http::write(advertiser_stream, root3);
    http::write(advertiser_stream, emb1);
    http::write(advertiser_stream, emb2);
    http::write(advertiser_stream, svc);
}
// note udp packets are also bounded in length (as small as 512 bytes)
// and each discovery message shall fit entirely in a single udp packet

// there is no guarantee that messages will arrive in a particular order

// the ttl for the ip packet should default to 2 and should be configurable



void SsdpServer::startReceive()
{
    listener_socket.async_receive_from(
        buffer,
        remote,
        [this](const boost::system::error_code& ec, std::size_t size) {
            handleReceive(ec, size);
        }
    );
}

void SsdpServer::handleReceive(const boost::system::error_code& ec, std::size_t size)
{
    // see: 1.3.2 Search request with M-SEARCH
    // M-SEARCH
    // - control point searching the network for devices
    // - requests of this method have no body
    // - TTL for this IP packet should default to 2 and should be configurable
    // - Response outlined in 1.3.3 Search response

    switch (request.method()) {
        case http::verb::msearch:
            handleSearch();
            break;
        default:
            // not what we're looking for
            break;
    }

    startReceive();
}

void SsdpServer::handleSearch()
{
    // ensure request line is valid
    if (request.method() != http::verb::msearch || request.target() != "*" || request.version() != 11) {
        return;
    }

    // request should not have a body
    if (false /*request has a body*/) {
        return;
    }

    bool is_multicast = remote.address().is_multicast();
    search_request sr;

    // fields required for both multicast and unicast
    try {
        sr.host = request.at("HOST");
        sr.man = request.at("MAN");
        sr.st = request.at("ST");
    } catch (std::out_of_range&) {
        return;
    }

    // fields required only for multicast
    if (is_multicast) {
        try {
            sr.mx = request.at("MX");
            sr.cpfn = request.at("CPFN.UPNP.ORG");
        } catch (std::out_of_range&) {
            return;
        }
    }

    // fields allowed for both multicast and unicast
    sr.user_agent = request["USER-AGENT"];
    
    // fields allowed only for multicast
    if (is_multicast) {
        sr.tcp_port = request["TCPPORT.UPNP.ORG"];
        sr.cpuuid = request["CPUUID.UPNP.ORG"];
    }

    // what about additional / unnecessary fields? ignore for now...
}

void SsdpServer::handleSend()
{
}

} // namespace net
