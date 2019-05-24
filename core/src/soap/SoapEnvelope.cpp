#include <string>
#include <sstream>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "SoapBody.hpp"
#include "SoapHeader.hpp"
#include "SoapEnvelope.hpp"

namespace pt = boost::property_tree;

namespace cppdlna {

SoapBody* SoapEnvelope::getBody()
{
    return &body;
}

SoapHeader* SoapEnvelope::getHeader()
{
    return &header;
}

pt::ptree SoapEnvelope::createPropertyTree()
{
    pt::ptree tree;
    tree.put("soap:Envelope.<xmattr>.xmlns:soap", "http://www.w3.org/2003/05/soap-envelope/");
    tree.put("soap:Envelope.<xmlattr>.soap:encodingStyle", "http://www.w3.org/2003/05/soap-encoding");
    if (!header.isEmpty()) {
        tree.put_child("soap:Envelope", header.createPropertyTree());
    }
    tree.put_child("soap:Envelope", body.createPropertyTree());
    return tree;
}

std::string SoapEnvelope::to_string()
{
    pt::ptree tree = createPropertyTree();
    std::stringstream ss;
    pt::write_xml(ss, tree);
    return ss.str();
}

} // namespace cppdlna
