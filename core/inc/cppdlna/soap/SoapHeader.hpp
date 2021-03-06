#ifndef SOAP_HEADER_HPP
#define SOAP_HEADER_HPP

#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>

namespace pt = boost::property_tree;

namespace cppdlna {

class SoapHeader
{
public:
    SoapHeader();
    bool isEmpty();
    void add(pt::ptree*);
    pt::ptree createPropertyTree();
    std::string to_string();

private:
    std::vector<pt::ptree*> elements;
};

} // namespace cppdlna

#endif
