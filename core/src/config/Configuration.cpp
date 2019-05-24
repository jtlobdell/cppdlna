#include <cppdlna/config/Configuration.hpp>
#include <cppdlna/config/Defaults.hpp>
#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <exception>

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

} // namespace cppdlna::config
