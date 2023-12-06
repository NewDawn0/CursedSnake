// Includes
#include "util/parser.h"
#include <iostream>

// Main function
int main (int argc, char** argv) {
    util::Arg arg0({"--help", "-h"});
    util::Arg arg1({"--version", "-v"});
    util::Arg arg2({"--train", "-t"}, nullptr, true, true);
    util::ArgParser parser = util::ArgParser()
        .configure({"SSnake", "A scuffed snake game", 0.1f})
        .add(arg0)
        .add(arg1)
        .add(arg2);
    auto res = parser.get<int>("-h");
    std::cout <<  parser.get<int>("-h").get() << std::endl;
    if (res.isNone()) std::cout << "Value is none" << std::endl;
    if (res.isSome()) std::cout << "Value is some : " << res.get() << std::endl;

    // auto res = parser.getArg<int>("-t");
    // auto dval = res.default_val.get();
    // std::cout << dval << std::endl;
    // logger::LOG_INFO("Should not print : info");
    // logger::LOG_ERROR("Shoudl print : err");
    // logger::LOG_DEBUG("Shoudl print : debug");
    return EXIT_SUCCESS;
}
