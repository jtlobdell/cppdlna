#include <map>
#include <string>
#include <stdexcept>

#include <cppdlna/config/Defaults.hpp>
#include <cppdlna/log/Log.hpp>

namespace config {
namespace defaults {

namespace {

std::map<std::string, std::string> defaults =
{
    {"interface", "0.0.0.0"},
    {"ssdp.port", "1900"},
    {"ssdp.advertisement.address", "239.255.255.250"},
    {"ssdp.advertisement.port", "1900"},
    {"ssdp.advertisement.age", "1800"},
    {"ssdp.advertisement.initial_discoveries", "3"},
    {"ssdp.advertisement.initial_discoveries_spacing_ms", "200"}
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

} // namespace defaults
} // namespace config
