#include <cppdlna/net/SsdpUdpListener.hpp>
#include <cppdlna/config/Configuration.hpp>

namespace net {
namespace ssdp {

namespace asio = boost::asio;
namespace ip = boost::asio::ip;
namespace udp = boost::asio::ip::udp;

UdpListener::UdpListener(boost::asio::io_context& io_context)
    : socket(io_context,
             udp::endpoint
             (
                 ip::address::make_address(config::getSetting("interface")),
                 std::stoul(config::getSetting("udpListenerPort"))
             ))                 
{
}

void UdpListener::run()
{
    // advertise
    startReceive(); // handle searches
}

void UdpListener::startReceive()
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

void UdpListener::handleReceive()
{
    // see: 1.3.2 Search request with M-SEARCH
    // M-SEARCH
    // - control point searching the network for devices
    // - requests of this method have no body
    // - TTL for this IP packet should default to 2 and should be configurable
    // - Response outlined in 1.3.3 Search response

    switch (reqiest.method()) {
        case http::verb::m_search: // doubt this exists...
            break;
            
    }
}

void UdpListener::handleSend()
{
}

} // namespace ssdp
} // namespace net
