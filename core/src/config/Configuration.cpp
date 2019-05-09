#include <cppdlna/config/Configuration.hpp>
#include <cppdlna/config/Defaults.hpp>

#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <exception>
namespace pt = boost::property_tree;

namespace config {
namespace {
pt settings;
} // anonymous namespace

void load(std::string filename)
{
    pt::read_ini(filename, settings);
}

std::string get(std::string settingName)
{
    return setings.get(settingName, config::defaults::get(settingName));
}

} // namespace config
