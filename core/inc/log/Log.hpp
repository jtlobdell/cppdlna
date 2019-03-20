#ifndef CPPDLNA_LOG_LOG_HPP
#define CPPDLNA_LOG_LOG_HPP

namespace cppdlna {
namespace log {

void init();
void setLogFile(std::string);

void trace(std::string);
void debug(std::string);
void info(std::string);
void warning(std::string);
void warning(const std::exception&);
void warning(std::string, const std::exception&);
void error(std::string);
void error(const std::exception&);
void error(std::string, const std::exception&);
void fatal(std::string);
void fatal(const std::exception&);
void fatal(std::string, const std::exception&);

} // namespace log
} // namespace cppdlna

#endif
