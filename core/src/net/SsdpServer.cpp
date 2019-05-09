#include <cppdlna/net/SsdpServer.hpp>
#include <cppdlna/config/Configuration.hpp>

namespace net {

namespace asio = boost::asio;
namespace ip = boost::asio::ip;
namespace udp = boost::asio::ip::udp;

// http over udp with beast?

SsdpServer::SsdpServer(boost::asio::io_context& io_context)
    : socket(io_context,
             udp::endpoint
             (
                 ip::address::make_address(config::get("interface")),
                 std::stoul(config::get("udpListenerPort"))
             ))                 
{
}

void SsdpServer::run()
{
    startAdvertise(); // advertise --> run as a new thread
    startReceive(); // handle searches
}

void SsdpServer::stop()
{
    // stop the loop in the startAdvertise thread
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

    http::header<true> root1, root2, root3, emb1, emb2, svc;
    root1.method(http::verb::notify);
    root1.target("*");
    // note -- newly constructed headers use HTTP/1.1 by default, so we don't need to set it here.
    root1.set("CACHE-CONTROL", "max-age=" + config::get("ssdp.advertisement.age"));
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
    // send(root1), send(root2), ...

    // due to the unreliable nature of udp devices should send the entire
    // set of discovery messages more than once with some delay between
    // the sets e.g. a few hundred ms. to avoid congestion they shouldn't
    // be send more than three times.

    // for (i = 1; i < config["number of ssdp initial discovery sends"]; i++) {
    //   sleep(200ms)
    //   send(root1), send(root2), ...
    // }

    // in addition, device shall re-send advertisements periodically
    // prior to expiration. it is recommended that such refreshing of the
    // advertisements be done at a randomly-distributed interval of less
    // than one-half of the advertisement expiration time, so as to provide
    // the opportunity for recovery from lost advertisements before the
    // advertisement expires, and to distribute over time the advertisement
    // of multiple devices on the network in order to avoid spikes in
    // network traffic

    // create a queue of messages and when to re-send advertisements
    // queue<tuple<header,time_to_send>> ad_q
    // spacing = (exp_time / 2) / num_ads. probably (exp_time/2) / 6 = exp_time / 12
    // queue.push(root1, now + spacing * 1)
    // queue.push(root2, now + spacing * 2)
    // queue.push(root3, now + spacing * 3)
    // queue.push(emb1, now + spacing * 4)
    // queue.push(emb2, now + spacing * 5)
    // queue.push(emb2, now + spacing * 6)
    // 
    // while (running) {
    //   h, t = ad_q.front()
    //   ad_q.pop()
    //   sleep_until(t)
    //   send(h)
    //   new_h = h, with any updates
    //   new_t = h + exp_time/2 + randomness
    //   ad_q.push(new_h,new_t)
    // }

    // now the server is killed. notify with ssdp:byebye
    
    root1.set("NTS", "ssdp:byebye");
    root2.set("NTS", "ssdp:byebye");
    root3.set("NTS", "ssdp:byebye");
    emb1.set("NTS", "ssdp:byebye");
    emb2.set("NTS", "ssdp:byebye");
    svc.set("NTS", "ssdp:byebye");

    root1.clear("CACHE-CONTROL");
    root1.clear("LOCATION");
    root1.clear("SERVER");
    root1.clear("SEARCHPORT.UPNP.ORG");
    
    root2.clear("CACHE-CONTROL");
    root2.clear("LOCATION");
    root2.clear("SERVER");
    root2.clear("SEARCHPORT.UPNP.ORG");
    
    root3.clear("CACHE-CONTROL");
    root3.clear("LOCATION");
    root3.clear("SERVER");
    root3.clear("SEARCHPORT.UPNP.ORG");

    emb1.clear("CACHE-CONTROL");
    emb1.clear("LOCATION");
    emb1.clear("SERVER");
    emb1.clear("SEARCHPORT.UPNP.ORG");
    
    emb2.clear("CACHE-CONTROL");
    emb2.clear("LOCATION");
    emb2.clear("SERVER");
    emb2.clear("SEARCHPORT.UPNP.ORG");

    svc.clear("CACHE-CONTROL");
    svc.clear("LOCATION");
    svc.clear("SERVER");
    svc.clear("SEARCHPORT.UPNP.ORG");

    // send(root1), send(root2), ...
}
// note udp packets are also bounded in length (as small as 512 bytes)
// and each discovery message shall fit entirely in a single udp packet

// there is no guarantee that messages will arrive in a particular order

// the ttl for the ip packet should default to 2 and should be configurable



void SsdpServer::startReceive()
{
    socket.async_receive_from(
        boost::asio::buffer(rx_buffer),
        remote,
        boost::bind(
            &UdpListener::handleReceive,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );
}

void SsdpServer::handleReceive()
{
    // see: 1.3.2 Search request with M-SEARCH
    // M-SEARCH
    // - control point searching the network for devices
    // - requests of this method have no body
    // - TTL for this IP packet should default to 2 and should be configurable
    // - Response outlined in 1.3.3 Search response

    switch (request.method()) {
        case http::verb::msearch:
            break;
        default:
            break;
            
    }
}

void SsdpServer::handleSend()
{
}

} // namespace net
