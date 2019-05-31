#include <cppdlna/upnp/Uuid.hpp>
#include <cppdlna/config/Configuration.hpp>
#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace cppdlna::upnp {

namespace {

std::string uuid = "";

std::string generate_uuid()
{
    return boost::uuids::to_string(boost::uuids::random_generator()());
}

std::string get_generated_uuid()
{
    if (uuid == "") uuid = generate_uuid();
    return uuid;
}


} // anonymous namespace

std::string getUuidString()
{
    if (config::get_bool("uuid.use_config")) {
        return config::get("uuid.value");
    } else {
        return get_generated_uuid();
    }
}

} // namespace cppdlna::upnp
