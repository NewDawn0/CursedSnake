// Includes
#include <cstdlib>

#include "util/parser.h"

using namespace types;

// Main function
int main(const int argc, const char** argv) {
    util::ArgParser<std::string> parser =
        util::ArgParser<std::string>()
            .flagBool({"--help", "-h"}, "The help message")
            .flagBool({"--version", "-v"}, "The help message")
            .flagBool({"--ai"}, "Enable AI steering")
            .flagInt({"--train", "-t"},
                     "The amount of threads the training should use", 8)
            .flagVec2({"--screen-size", "-ss"},
                      "Sets the size of the game area", "2000x4000")
            .as_result()
            .expect("Failed to instanciate arguments");
    auto parsing_result = parser.parse(argc, argv).as_result();
    if (!parsing_result) {
        std::cerr << parsing_result.unwrap_err() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
