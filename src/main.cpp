// Includes
#include "parser.h"
#include "logger.h"
#include <cstdlib>

// Main function
int main (int argc, char** argv) {
    parser::ArgParser parser;
    parser.configure({"SSnake", "A scuffed snake game", 0.1f})
        .add({{"--help", "-h"}, "false", true, false})
        .add({{"--version", "-v"}, "false", true, false})
        .add({{"--verbose", "-vb"}, "false", true, false})
        .parse(argc, argv);
    logger::LOG_INFO("Should not print : info");
    logger::LOG_ERROR("Shoudl print : err");
    logger::LOG_DEBUG("Shoudl print : debug");
    return EXIT_SUCCESS;
}
