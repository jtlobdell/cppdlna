#ifndef SOAP_FAULT_HPP
#define SOAP_FAULT_HPP

#include <string>
#include <boost/property_tree/ptree.hpp>

namespace pt = boost::property_tree;

enum SoapFaultCode {
    VersionMismatch,
    MustUnderstand,
    Client,
    Server
};

class SoapFault
{
public:
    SoapFault() = delete;
    explicit SoapFault(SoapFaultCode, std::string);
    explicit SoapFault(SoapFaultCode, std::string, std::string);
    explicit SoapFault(SoapFaultCode, std::string, pt::ptree*);
    explicit SoapFault(SoapFaultCode, std::string, std::string, pt::ptree*);    
    pt::ptree createPropertyTree();
    std::string to_string();

private:
    SoapFaultCode faultCode;
    std::string faultString;
    std::string faultActor;
    pt::ptree* detail;
};

#endif
