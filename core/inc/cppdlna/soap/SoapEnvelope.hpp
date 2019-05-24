#ifndef SOAP_ENVELOPE_HPP
#define SOAP_ENVELOPE_HPP

#include <string>
#include <boost/property_tree/ptree.hpp>
#include "SoapBody.hpp"
#include "SoapHeader.hpp"

namespace pt = boost::property_tree;

namespace cppdlna {

class SoapEnvelope
{
public:
    SoapBody* getBody();
    SoapHeader* getHeader();
    pt::ptree createPropertyTree();
    std::string to_string();
private:
    SoapBody body;
    SoapHeader header;
};

} // namespace cppdlna

#endif
