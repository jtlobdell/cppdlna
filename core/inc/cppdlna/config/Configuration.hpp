#ifndef CPPDLNA_CONFIG_CONFIGURATION_HPP
#define CPPDLNA_CONFIG_CONFIGURATION_HPP

#include <string>

namespace cppdlna::config {

void load(std::string);
std::string get(std::string);

} // namespace cppdlna::config

#endif
