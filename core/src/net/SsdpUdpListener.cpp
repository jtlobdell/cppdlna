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
    startReceive();
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
    
}

void UdpListener::handleSend()
{
}

} // namespace ssdp
} // namespace net
