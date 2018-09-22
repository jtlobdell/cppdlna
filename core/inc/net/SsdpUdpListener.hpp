#ifndef CPPDLNA_NET_SSDP_UDP_LISTENER_HPP
#define CPPDLNA_NET_SSDP_UDP_LISTENER_HPP

namespace net {
namespace ssdp {


// Listen for SSDP UDP packets sent on 239.255.255.250, port 1900
class UdpListener
{
public:
    UdpListener(boost::asio::io_context&);
    void run();
private:
    void startReceive();
    void handleReceive();
    void handleSend();

    boost::asio::ip::udp::socket socket;
    boost::asio::ip::udp::endpoint remote;
    // buffer...
}; // class UdpListener

} // namespace ssdp
} // namespace net

#endif
