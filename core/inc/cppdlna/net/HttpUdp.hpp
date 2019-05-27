#ifndef CPPDLNA_NET_HTTP_UDP
#define CPPDLNA_NET_HTTP_UDP

#include <boost/asio.hpp>
#include <boost/beast/http.hpp>
#include <ostream>

namespace asio = boost::asio;
namespace http = boost::beast::http;

namespace cppdlna::net {

// datagram sockets don't conform to the Stream concept so we can't use http::write().
// this implements similar functionality.
template <
    typename Protocol, typename Executor,
    bool isRequest, typename Body, typename Fields
    >
void write_message_to_udp(asio::basic_datagram_socket<Protocol, Executor>& sock,
                          http::message<isRequest, Body, Fields> const& msg,
                          typename Protocol::endpoint const& remote)
{
    boost::system::error_code ec;
    asio::streambuf b;
    std::ostream os(&b);
    http::serializer<isRequest, Body, Fields> sr{msg};

    // serialize msg to a streambuf
    do {
        sr.next(ec,
                [&sr,&os](boost::system::error_code& ec, auto const& buffers) {
                    ec = {};
                    std::size_t bytes_transferred = 0;

                    for (auto it = asio::buffer_sequence_begin(buffers);
                         it != asio::buffer_sequence_end(buffers); ++it) {
                        asio::const_buffer const buffer = *it;
                        
                        os.write(reinterpret_cast<char const*>(buffer.data()), buffer.size());
                        
                        if (os.fail()) {
                            ec = make_error_code(boost::system::errc::io_error);
                            return;
                        }

                        bytes_transferred += asio::buffer_size(buffer);
                    }
                    sr.consume(bytes_transferred);
                }
        );
    } while (!sr.is_done() && !ec);

    // if error code is set quit and report the error
    if (ec) {
        throw boost::system::system_error(ec, ec.message());
    }

    // buf now contains the serialized message, so send it.
    sock.send_to(b.data(), remote);
}

} // namespace cppdlna::net

#endif
