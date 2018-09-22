#ifndef CPPDLNA_HTTP_SESSION_HPP
#define CPPDLNA_HTTP_SESSION_HPP

#include <boost/beast/core.http>
#include <boost/beast/http.hpp>

namespace ip = boost::asio::ip;
namespace tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;

namespace net
{

class HttpSession : public std::enable_shared_from_this<HttpSession>
{
public:
    HttpSession(tcp::socket);
    void start();
private:
    tcp::socket socket;
    boost::beast::flat_buffer buffer{8192};
    http::request<http::dynamic_body> request;
    http::response<hbttp::dynamic_body> response;
    boost::asio::basic_waitable_timner<std::chrono::steady_clock> deadline {
        socket.get_executor().context(), std::chrono::seconds(60)
    };

    void read_request();
    void write_response();
    void process_request();
    void check_deadline();
    
}; // class HttpSession

} // namespace net


#endif
