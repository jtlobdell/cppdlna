#include <map>
#include <string>
#include <stdexcept>

#include <cppdlna/config/Defaults.hpp>
#include <cppdlna/log/Log.hpp>

namespace cppdlna::config::defaults {

namespace {

std::map<std::string, std::string> defaults =
{
    {"ssdp.interface", "192.168.1.100"},
    {"ssdp.port", "1900"},
    {"ssdp.multicast_address", "239.255.255.250"},
    {"ssdp.advertisement.age", "1800"},
    {"ssdp.advertisement.initial_discoveries", "3"},
    {"ssdp.advertisement.initial_discoveries_spacing_ms", "200"},

    {"description.os_version", "Linux/5.1.5"},
    {"description.product_version", "cppdlna/0.1"},

    {"uuid.use_config", "false"}
    // uuid.value --> expected in config file if above is true
    
}; // std::map defaults

} // anonymous namespace

std::string get(std::string settingName)
{
    try {
        return defaults.at(settingName);
    } catch (std::out_of_range&) {
        cppdlna::log::fatal("Setting not found: " + settingName);
        throw std::runtime_error("Setting not found: " + settingName);
    }
}

} // namespace cppdlna::config::defaults
