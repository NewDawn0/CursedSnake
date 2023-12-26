// @AUTHORS: NewDawn0
// @DATE: 2023-12-24
// @FILE: util/parser/arg-utils.h
// @TYPE: Header only
// @DESC: Argument parser utilities

// Header guard
#ifndef ARG_H
#define ARG_H

// Includes
#include <sys/types.h>

#include <any>
#include <cstddef>
#include <exception>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>

#include "../../types/option.h"
#include "../../types/result.h"

// Namespaces
namespace util {

// 2d scalar type
typedef struct {
    uint x, y;
} vec2;
// Types of flags the parser accepts
enum FlagType {
    Bool,
    Int,
    Str,
    Vec2,
};
// Flag def
struct Flag {
    types::Option<std::any> value;
    FlagType type;
};

// Mainly a wrapper struct used by ArgParse::parse()
struct FlagParse {
    const char* key;
    Flag flag;
};

// From and to any used in flag
template <typename T>
types::Option<std::any> as_any(const types::Option<T>& in) {
    types::Option<std::any> res = {};
    if (in) res = std::any(in.unwrap());
    return res;
}
template <typename T>
types::Option<T> from_any(const types::Option<std::any>& in) {
    types::Option<T> res = {};
    if (in) res = std::any_cast<T>(in.unwrap());
    return res;
}

template <typename E>
types::Result<util::vec2, E> as_vec2(const std::string& input,
                                     const char& delimiter) noexcept {
    std::string trimput = std::regex_replace(input, std::regex(" "), "");
    if (delimiter != '-' && trimput.find('-') != std::string::npos)
        return types::Err<E>("Invalid vec2 format, values must be positive");
    size_t delimiterPos = trimput.find(delimiter);
    if (delimiterPos == std::string::npos ||
        trimput.find(delimiter, delimiterPos + 1) != std::string::npos) {
        return types::Err<E>("Invalid vec2 format, use HEIGHTxWIDTH");
    }
    try {
        util::vec2 res;
        res.x = std::stoul(trimput.substr(0, delimiterPos));
        res.y = std::stoul(trimput.substr(delimiterPos + 1));
        return res;
    } catch (const std::invalid_argument& ia) {
        return types::Err<E>(ia.what());
    } catch (const std::out_of_range) {
        return types::Err<E>("Value of vec2 is out of range");
    } catch (const std::exception& e) {
        return types::Err<E>(e.what());
    }
}

// Flag type map for type safety
template <FlagType>
struct FlagTypeMap;
template <>
struct FlagTypeMap<Bool> {
    using type = bool;
};
template <>
struct FlagTypeMap<Int> {
    using type = int;
};
template <>
struct FlagTypeMap<Str> {
    using type = std::string;
};
template <>
struct FlagTypeMap<Vec2> {
    using type = vec2;
};

}  // namespace util
#endif  // !ARG_H
