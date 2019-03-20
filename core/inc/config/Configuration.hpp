#ifndef CPPDLNA_CONFIG_CONFIGURATION_HPP
#define CPPDLNA_CONFIG_CONFIGURATION_HPP

#include <string>

namespace config {

void readConfigFile(std::string);
std::string getSetting(std::string);

} // namespace config

#endif
