
#include <string>

namespace Upnp {

std::string buildResponseHeader(int responseCode, std::string responseMessage, int bodyLen)
{
    std::string httpResponseHeader = "";

    httpResponseHeader += "HTTP/1.1" + " ";
    httpResponseHeader += std::to_string(responseCode) + " ";
    httpResponseHeader += responseMessage + " " + "\r\n";
    httpResponseHeader += "Content-Type: "; // + ("text/html" or "text/xml; charset=\"utf-8\"") + "\r\n";
    httpResponseHeader += "Connection: " + "close" + "\r\n"; // keep-alive / open?
    httpResponseHeader += "Content-Length: " + std::to_string(bodyLen) + "\r\n";
    httpResponseHeader += "Server: " + /* server string */ + "\r\n";
    if (/* timeout */)
        httpResponseHeader += "Timeout: Second-" + std::to_string(/* request timeout */) + "\r\n";
    if (/* SID */)
        httpResponseHeader += "SID: " + /* not sure, see upnphttp.c line 1143ish */ + "\r\n";
    if (/* language */)
        httpResponseHeader += "Content-Language: en\r\n"; // support for other languages maybe?
    httpResponseHeader += "Date: " + /* date string */ + "\r\n";
    httpResponseHeader += "EXT:\r\n";
    httpResponseHeader += "\r\n";
}

// need SSDP udp listener socket
// minissdp.c, line94:
// "Open and configure the socket listening for SSDP udp packets
// sent on 239.255.255.250 port 1900"

// need SSDP notification socket
// minissdp.c, line 122:
// "open the UDP socket used to send SSDP notifications to the
// multicast group reserved for them"



} // namespace Upnp
