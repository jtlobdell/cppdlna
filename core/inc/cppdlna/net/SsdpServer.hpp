#ifndef CPPDLNA_NET_SSDP_UDP_LISTENER_HPP
#define CPPDLNA_NET_SSDP_UDP_LISTENER_HPP

#define BOOST_BEAST_USE_STD_STRING_VIEW

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/thread.hpp>
#include <array>

namespace asio = boost::asio;
namespace ip = boost::asio::ip;
using udp = boost::asio::ip::udp;
namespace http = boost::beast::http;
using udp_stream = boost::beast::basic_stream<udp>;

namespace cppdlna::net {

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
    void handleReceive(const boost::system::error_code&, std::size_t);
    void handleSearch(http::message<true, http::empty_body> const&);

    udp::socket ssdp_socket;

    ip::address ssdp_interface;
    unsigned long multicast_port;
    ip::address multicast_addr;
    udp::endpoint tx_remote;

    std::array<char, 1024> rx_buf;
    udp::endpoint rx_remote;
    boost::thread advertiser_thread;
    
}; // class SsdpServer

} // namespace cppdlna::net

#endif
