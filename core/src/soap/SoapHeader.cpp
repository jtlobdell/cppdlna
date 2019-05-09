#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "SoapHeader.hpp"

namespace pt = boost::property_tree;

SoapHeader::SoapHeader()
{
}

bool SoapHeader::isEmpty()
{
    return elements.size() == 0;
}

void SoapHeader::add(pt::ptree* pt)
{
    elements.push_back(pt);
}

pt::ptree SoapHeader::createPropertyTree()
{
    pt::ptree tree;
    if (elements.size() == 0) {
        tree.put("SOAP-ENV:Header", "");
    } else {
        for (auto el: elements) {
            tree.add_child("SOAP-ENV:Header", *el);
        }
    }
    return tree;
}

std::string SoapHeader::to_string()
{
    pt::ptree tree = createPropertyTree();
    std::stringstream ss;
    pt::write_xml(ss, tree);
    return ss.str();
}
