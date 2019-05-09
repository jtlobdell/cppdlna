#ifndef CPPDLNA_NET_SSDP_UDP_LISTENER_HPP
#define CPPDLNA_NET_SSDP_UDP_LISTENER_HPP

#include <boost/thread/thread.hpp>

namespace ip = boost::asio::ip;
namespace udp = boost::asio::ip::udp;
namespace http = boost::beast::http;
using udp_stream = http::basic_stream<udp>;

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

    udp::socket listener_socket;
    udp_stream advertiser_stream;
    udp::endpoint remote;
    boost::beast::flat_buffer buffer{8192};
    http::request<http::dynamic_body> request;
    http::response<http::dynamic_body> response;
    boost::thread advertiser_thread;
}; // class SsdpServer

} // namespace net

#endif
