#include <cppdlna/net/SsdpServer.hpp>
#include <boost/asio.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    try {
        boost::asio::io_context ioc{1};
        cppdlna::net::SsdpServer ssdp_server(ioc);
        ssdp_server.run();
        ioc.run();
    } catch (std::exception& e) {
        std::cerr << "Exception:" << std::endl << e.what() << std::endl;
    }
    
    return 0;
}
