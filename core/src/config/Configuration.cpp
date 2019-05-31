#include <cppdlna/config/Configuration.hpp>
#include <cppdlna/config/Defaults.hpp>
#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <exception>
#include <boost/algorithm/string/case_conv.hpp>

namespace pt = boost::property_tree;

namespace cppdlna::config {

namespace {
pt::ptree settings;
} // anonymous namespace

void load(std::string filename)
{
    pt::read_ini(filename, settings);
}

std::string get(std::string settingName)
{
    return settings.get(settingName, config::defaults::get(settingName));
}

bool get_bool(std::string settingName)
{
    std::string s = get(settingName);
    boost::to_lower(s);
    
    if (s == "1" || s == "true") {
        return true;
    } else if (s == "0" || s == "false") {
        return false;
    } else {
        throw std::runtime_error("Failed to parse setting: " + settingName);
    }
}


} // namespace cppdlna::config
