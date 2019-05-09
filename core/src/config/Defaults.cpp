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
    {"ssdp.advertisement.age", "1800"}
} // std::map defaults

} // anonymous namespace

std::string get(std::string settingName)
{
    try {
        return defauts.at(settingName);
    } catch (std::out_of_range&) {
        cppdlna::log::fatal("Setting not found: " + settingName);
        throw std::runtime_error("Setting not found: " + settingName);
    }
}

} // namespace defaults
} // namespace config
