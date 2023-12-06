// @AUTHORS: NewDawn0
// @DATE: 2023-11-24
// @FILE: util/parser.h
// @TYPE: Header only
// @DESC: Argument parser

// Header guard
#ifndef PARSER_H
#define PARSER_H

// Includes
#include <memory>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <any>
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include "../types/option.h"

// Namespaces
namespace util {

// Parser config
struct AppConfig {                                                // Parser configuration:
    std::string app_name;                                         // Configures some global app options
    std::string app_desc;                                         // Used in the help and version arguments
    float app_vers = 0.0f;
    bool use_default_fns = true;
    bool ignore_invalid_options = false;
};

// Cli argument
struct ArgNoNames {                                               // A CLI argument without the argument aliases:
    std::any default_val;                                         // Contains inforamtion about the argument like:
    bool is_optional;                                             // If it is optional or whether it takes values
    bool takes_value;                                             // and if it has a default value
    bool has_default_val;
    ArgNoNames(bool is_optional = true, bool takes_value = true)
        noexcept : default_val(nullptr), is_optional(is_optional), takes_value(takes_value), has_default_val(false) {}
    template<typename T>
    ArgNoNames(T default_val, bool is_optional = true, bool takes_value = true)
        noexcept : default_val(default_val), is_optional(is_optional), takes_value(takes_value), has_default_val(false) {}
};
struct Arg : public ArgNoNames {                                  // Argument with name aliases
    std::vector<std::string> names;                               // Same as ArgNoNames with the addition
    Arg(std::initializer_list<std::string> names,                 // of the name aliases wich is the vector of strings
        bool is_optional = true, bool takes_value = true)
        noexcept : ArgNoNames(is_optional, takes_value), names(names) {
        // Some assert tickery as to asser a non static value
        assert(("Names must have at least one argument", names.size() > 1));
    }
    template<typename T>
    Arg(std::initializer_list<std::string> names, T val,          // of the name aliases wich is the vector of strings
        bool is_optional = true, bool takes_value = true)
        noexcept : ArgNoNames(val, is_optional, takes_value), names(names) {
        // Some assert tickery as to asser a non static value
        assert(("Names must have at least one argument", names.size() > 1));
    }
};

// The parser itself
class ArgParser {
private:
    AppConfig app_config;
    std::vector<std::string> otherArgs;
    std::unordered_map<std::string, std::string> arg_aliases;     // Key to primary key translation
    std::unordered_map<std::string, ArgNoNames> arg_values;       // Primary key translation to Arg
public:
    // Parser Configuration
    ArgParser& configure(const AppConfig& app_config) noexcept {  // Configures the parser using the AppConfig
        this->app_config = app_config;                            // Takes an AppConfig or its initalizer list as the argument
        return *this;
    }
    AppConfig& appInfo() noexcept {                               // Returns a reference to its inner app_config
        return app_config;
    }

    // Argument Configuration
    ArgParser& add(const Arg& arg) noexcept {                              // Add new arguments
        for (const std::string& alias : arg.names) {
            arg_aliases[alias] = arg.names[0];                    // For each name translate to primary
        }
        arg_values[arg.names[0]] = {arg.default_val, arg.is_optional, arg.takes_value};
        return *this;
    }
    template<typename T>
    types::Option<T> get(const std::string name) const noexcept {
        try {
            ArgNoNames arg_nn = arg_values.at(arg_aliases.at(name));
            if (!arg_nn.has_default_val) return types::Option<T>::None();
            return types::Option<T>::Some(std::any_cast<T>(arg_nn.default_val));
        } catch (...) {
            // Yeet formatted error
            assert(("Invalid argument `"+ name +"`", false));
        }
    }
    std::vector<std::string> getOther() const noexcept {
        assert(("Other arguments set to ignore which disables this option", !app_config.ignore_invalid_options));
        return otherArgs;
    }

    ArgParser& parse(int argc, char** argv) {
        // TODO:
        return *this;
    };
};

}       // !namespace util
#endif  // !PARSER_H
