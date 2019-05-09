#include <string>
#include <sstream>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "SoapFault.hpp"

namespace {

std::string faultCodeToString(SoapFaultCode faultCode)
{
    switch (faultCode) {
        case VersionMismatch:
            return "soap:VersionMismatch";
            break;
        case MustUnderstand:
            return "soap:MustUnderstand";
            break;
        case Client:
            return "soap:Client";
            break;
        case Server:
            return "soap:Server";
            break;
    }
}

} // anonymous namespace

namespace pt = boost::property_tree;

SoapFault::SoapFault(SoapFaultCode faultCode, std::string faultString)
    : faultCode(faultCode), faultString(faultString), detail(nullptr)
{
}

SoapFault::SoapFault(SoapFaultCode faultCode, std::string faultString, std::string faultActor)
    : faultCode(faultCode), faultString(faultString), faultActor(faultActor), detail(nullptr)
{
}

SoapFault::SoapFault(SoapFaultCode faultCode, std::string faultString, pt::ptree* detail)
    : faultCode(faultCode), faultString(faultString), detail(detail)
{
}

SoapFault::SoapFault(SoapFaultCode faultCode, std::string faultString, std::string faultActor, pt::ptree* detail)
    : faultCode(faultCode), faultString(faultString), faultActor(faultActor), detail(detail)
{
}

pt::ptree SoapFault::createPropertyTree()
{
    pt::ptree tree;
    
    tree.put("soap:Fault.faultcode.<xmlattr>.xsi:type", "xsd:string");
    tree.put("soap:Fault.faultcode", faultCodeToString(faultCode));

    tree.put("soap:Fault.faultstring.<xmlattr>.xsi:type", "xsd:string");
    tree.put("soap:Fault.faultstring", faultString);

    if (faultActor != "") {
        tree.put("soap:Fault.faultactor.<xmlattr>.xsi:type", "xsd:string");
        tree.put("soap:Fault.faultactor", faultActor);        
    }

    if (detail != nullptr) {
        tree.put_child("soap:Fault.detail", *detail);
    }

    return tree;
}

std::string SoapFault::to_string()
{
    pt::ptree tree = createPropertyTree();
    std::stringstream ss;
    pt::write_xml(ss, tree);
    return ss.str();
}

/*
int main(int argc, char* argv[])
{
    SoapFault testFault1(VersionMismatch, "Test Fault 1");
    SoapFault testFault2(MustUnderstand, "Test Fault 2", "Testing a second fault");
    pt::ptree tree;
    tree.add("test","testing");
    tree.add("test","testing");
    SoapFault testFault3(Client, "Test Fault 3", &tree);
    SoapFault testFault4(Server, "Test Fault 4", "Testing a 4th fault", &tree);

    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Test Fault 1:" << std::endl;
    std::cout << testFault1.to_string() << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Test Fault 2:" << std::endl;
    std::cout << testFault2.to_string() << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Test Fault 3:" << std::endl;
    std::cout << testFault3.to_string() << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Test Fault 4:" << std::endl;
    std::cout << testFault4.to_string() << std::endl;
    std::cout << "--------------------------------------" << std::endl;
}
*/

