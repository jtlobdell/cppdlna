#include <cppdlna/net/SsdpUdpListener.hpp>

namespace net {
namespace ssdp {

UdpListener::UdpListener(boost::asio::io_context& io_context)
    : socket(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 1900))
{
    // to-do: specific interface? different port? ipv6?
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
