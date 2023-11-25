// @AUTHORS: NewDawn0
// @DATE: 2023-11-24
// @FILE: parser.h
// @TYPE: Header only
// @DESC: Argument parser

// Header guard
#ifndef PARSER_H
#define PARSER_H

// Includes
#include "types.h"
#include <any>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>

// Namespaces
namespace parser {

// Parser config
struct Config {
    std::string app_name;
    std::string app_desc;
    float app_vers = 0.0f;
    bool use_default_fns = true;
    bool ignore_invalid_options = false;
};

// Cli argument
struct Argument {
    std::vector<std::string> names;
    std::optional<std::any> default_value;
    bool has_optional_args = false;
    bool has_more_args = false;
};

// The parser itself
class ArgParser {
private:
    Config config;
    std::map<std::string, Argument> args;
    std::map<std::string, std::string> parsed_args;
public:
    ArgParser& configure(const Config& config) {
        this->config = config;
        return *this;
    }
    ArgParser& add(const Argument& arg) {
    for (const auto& name : arg.names) {
            args[name] = arg;
        }
        return *this;
    }
    ArgParser& parse(int argc, char** argv) {
        return *this;
    }
    types::Result<std::string, types::Error> get(const std::string& arg_name) {
        if (parsed_args.find(arg_name) != parsed_args.end()) {
            return types::Result<std::string, types::Error>::Ok(parsed_args[arg_name]);
        }
        return types::Result<std::string, types::Error>::Err("Argument not found");
    }
};

}       // !namespace parser
#endif  // !PARSER_H
