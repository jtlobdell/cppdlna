#ifndef CPPDLNA_NET_SSDP_UDP_LISTENER_HPP
#define CPPDLNA_NET_SSDP_UDP_LISTENER_HPP

namespace ip = boost::asio::ip;
namespace udp = boost::asio::ip::udp;
namespace http = boost::beast::http;

namespace net {

// Listen for SSDP UDP packets sent on 239.255.255.250, port 1900
class SsdpServer
{
public:
    SsdpServer(boost::asio::io_context&);
    void run();
    void stop();
private:
    void startAdvertise();
    void startReceive();
    void handleReceive();
    void handleSend();

    udp::socket socket;
    udp::endpoint remote;
    boost::beast::flat_buffer buffer{8192};
    http::request<http::dynamic_body> request;
    http::response<http::dynamic_body> response;
}; // class SsdpServer

} // namespace net

#endif
