#include <cppdlna/upnp/SystemInfo.hpp>
#include <cppdlna/config/Configuration.hpp>
#include <boost/predef.h>
#include <string>

#if BOOST_OS_LINUX
#include <sys/utsname.h>
#endif

namespace cppdlna::upnp {

namespace {

bool info_set = false;
std::string os = "";
std::string version = "";

void set_info()
{
#if BOOST_OS_LINUX
    utsname system_info;
    uname(&system_info);
    os = std::string(system_info.sysname);
    version = std::string(system_info.release);
#else
#error "OS not recognized; add it."    
#endif
}

} // anonymous namespace

std::string get_os()
{
    if (config::get_bool("description.get_os_from_config")) {
        return config::get("description.os");
    }
    
    if (!info_set) {
        set_info();
    }

    return os;
}

std::string get_os_version()
{
    if (config::get_bool("description.get_os_version_from_config")) {
        return config::get("description.os_version");
    }
    
    if (!info_set) {
        set_info();
    }

    return version;
}

} // namespace cppdlna::upnp
