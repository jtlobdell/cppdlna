#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "SoapBody.hpp"

namespace pt = boost::property_tree;

namespace cppdlna {

SoapBody::SoapBody()
{
}

void SoapBody::add(pt::ptree* pt)
{
    elements.push_back(pt);
}

pt::ptree SoapBody::createPropertyTree()
{
    pt::ptree tree;
    if (elements.size() == 0) {
        tree.put("soap:Body", "");
    } else {
        for (auto el: elements) {
            tree.add_child("soap:Body", *el);
        }
    }
    return tree;
}

std::string SoapBody::to_string()
{
    pt::ptree tree = createPropertyTree();
    std::stringstream ss;
    pt::write_xml(ss, tree);
    return ss.str();
}

} // namespace cppdlna
