#include <cppdlna/net/HttpSession.hpp>
#include <cppdlna/upnp/Soap.hpp>
#include <map>
#include <string>
#include <functional>


namespace ip = boost::asio::ip;
namespace tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;
namespace soap = cppdlna::upnp::soap;

namespace cppdlna {
namespace net {

HttpSession::HttpSession(tcp::socket s)
    : socket(std::move(s))
{
    soapMethods = {
        {"QueryStateVariable", soap::queryStateVariable},
        {"Browse", soap::browse},
        {"GetSearchCapabilities", soap::getSearchCapabilities},
        {"GetSortCapabilities", soap::getSortCapabilities},
        {"GetSystemUpdateID", soap::getSystemUpdateId},
        {"GetProtocolInfo", soap::getProtocolInfo},
        {"GetCurrentConnectionIDs", soap::getCurrentConnectionIds},
        {"GetCurrentConnectionInfo", soap::getCurrentConnectionInfo},
        {"IsAuthorized", soap::isAuthorized},
        {"RegisterDevice", soap::registerDevice},
        {"X_GetFeatureList", soap::x_getFeatureList},
        {"X_SetBookmark", soap::x_setBookmark}
    };
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
    // upnphttp.c L924
}

void HttpSession::process_head()
{
}

void HttpSession::process_post()
{
    // upnphttp.c, L684
    // executes the SOAP query if it is possible
    std::string soapAction = request.base["SOAPAction"];
    if (soapAction != "") {
        // process SOAP action
        // upnpsoap.c L1937-1995
        try {
            auto ftor = soapMethods.at(soapAction);
            ftor();
        } catch (std::out_of_range&) {
            // error 400 bad request, unknown SOAPAction
            
        }
    } else {
        // error 400 bad request, no SOAPAction in HTTP headers
    }
}

void HttpSession::process_subscribe()
{
    // upnphttp.c L762
    // subscribe or renew
}

void HttpSession::process_unsubscribe()
{
    // upnphttp.c L815
    // remove sub
}

void HttpSession::create_response()
{
    // more magic happens here
}

} // namespace net
} // namespace cppdlna

