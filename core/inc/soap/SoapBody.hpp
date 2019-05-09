#ifndef SOAP_BODY_HPP
#define SOAP_BODY_HPP

#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>

namespace pt = boost::property_tree;

class SoapBody
{
public:
    SoapBody();
    void add(pt::ptree*);
    pt::ptree createPropertyTree();
    std::string to_string();
    
private:
    std::vector<pt::ptree*> elements;
};

#endif
