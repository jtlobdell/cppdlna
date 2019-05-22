#include <cppdlna/log/Log.hpp>

#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <exception>
#include <string>
#include <boost/shared_ptr.hpp>

namespace cppdlna {
namespace log {

namespace {
boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;
boost::shared_ptr< boost::log::sinks::synchronous_sink< boost::log::sinks::text_ostream_backend >> console_sink;
} // anonymous namespace

void init()
{
    boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");
    console_sink = boost::log::add_console_log(
        std::cout,
        boost::log::keywords::format = "[%TimeStamp%] [%Severity%]: %Message%"
    );
    boost::log::add_common_attributes();
}

void setLogFile(std::string filename)
{
    boost::log::core::get()->remove_sink(console_sink);
    boost::log::add_file_log(
        filename,
        boost::log::keywords::format = "[%TimeStamp%] [%Severity%]: %Message%"
    );    
}

void trace(std::string msg)
{
    BOOST_LOG_SEV(lg, boost::log::trivial::trace) << msg;
}

void debug(std::string msg)
{
	BOOST_LOG_SEV(lg, boost::log::trivial::debug) << msg;
}

void info(std::string msg)
{
	BOOST_LOG_SEV(lg, boost::log::trivial::info) << msg;
}

void warning(std::string msg)
{
    BOOST_LOG_SEV(lg, boost::log::trivial::warning) << msg;
}

void warning(const std::exception& e)
{
    BOOST_LOG_SEV(lg, boost::log::trivial::warning) << e.what();
}

void warning(std::string msg, const std::exception& e)
{
    BOOST_LOG_SEV(lg, boost::log::trivial::warning) << msg;
    BOOST_LOG_SEV(lg, boost::log::trivial::warning) << e.what();
}

void error(std::string msg)
{
	BOOST_LOG_SEV(lg, boost::log::trivial::error) << msg;
}

void error(const std::exception& e)
{
	BOOST_LOG_SEV(lg, boost::log::trivial::error) << e.what();
}

void error(std::string msg, const std::exception& e)
{
	BOOST_LOG_SEV(lg, boost::log::trivial::error) << msg;
	BOOST_LOG_SEV(lg, boost::log::trivial::error) << e.what();
}

void fatal(std::string msg)
{
	BOOST_LOG_SEV(lg, boost::log::trivial::fatal) << msg;
}

void fatal(const std::exception& e)
{
	BOOST_LOG_SEV(lg, boost::log::trivial::fatal) << e.what();
}

void fatal(std::string msg, const std::exception& e)
{
	BOOST_LOG_SEV(lg, boost::log::trivial::fatal) << msg;
	BOOST_LOG_SEV(lg, boost::log::trivial::fatal) << e.what();
}

} // namespace log
} // namespace cppdlna
