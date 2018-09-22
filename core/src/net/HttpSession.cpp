#include <cppdlna/net/HttpSession.hpp>

namespace ip = boost::asio::ip;
namespace tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;

namespace net {

HttpSession::HttpSession(tcp::socket s)
    : socket(std::move(s))
{
}

void HttpSession::start()
{
    read_request();
    check_deadline();
}

void HttpSession::read_request()
{
    auto self = shared_from_this();

    http::async_read(
        socket,
        buffer,
        request,
        [self](boost::beast::error_code ec, std::size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);
            if (!ec) self->process_request();
        }
    );
}

void HttpSession::write_response()
{
    auto self = shared_from_this();

    // keep alive?
    http::async_write(
        s,
        response,
        [self](boost::beast::error_code ec, std::size_t) {
            self->socket.shutdown(tcp::socket::shutdown_send, ec);
            self->deadline.cancel();
        }
    );
}

void HttpSession::process_request()
{
    response.version(request.version());
    response.keep_alive(request.keep_alive());

    switch (request.method()) {
        case http::verb::get:
            process_get();
            break;
        case http::verb::head:
            process_head();
            break;
        case http::verb::post:
            process_post();
            break;
        case http::verb::subscribe:
            process_subscribe();
            break;
        case http::verb::unsubscribe:
            process_unsubscribe();
            break;
        default:
            break;
    }
}

void HttpSession::process_get()
{
}

void HttpSession::process_head()
{
}

void HttpSession::process_post()
{
}

void HttpSession::process_subscribe()
{
}

void HttpSession::process_unsubscribe()
{
}

void HttpSession::create_response()
{
    // more magic happens here
}

} // namespace net
