#include <iostream>
#include <string>
#include "SoapBody.hpp"
#include "SoapFault.hpp"

int main(int argc, char* argv[])
{
    SoapBody testBody1;
    SoapFault testFault1(VersionMismatch, "Shit's all fucked up, yo.");
    auto pt = testFault1.createPropertyTree();
    testBody1.add(&pt);
    SoapBody testBody2;
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Test Body 1:" << std::endl;
    std::cout << testBody1.to_string() << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Test Body 2:" << std::endl;
    std::cout << testBody2.to_string() << std::endl;
    std::cout << "--------------------------------------" << std::endl;

    return 0;
}
