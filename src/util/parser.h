// @AUTHORS: NewDawn0
// @DATE: 2023-12-25
// @FILE: util/parser.h
// @TYPE: Header only
// @DESC: Argument parser

// Header guard
#ifndef PARSER_H
#define PARSER_H

// Includes
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../types/option.h"
#include "../types/result.h"
#include "parser/arg-util.h"

// Namespaces
namespace util {

// The arg parser itself
template <typename T, typename E>
class ParseResultArgParser {
   protected:
    types::Result<T, E> inner;

   public:
    // Constructors & destructors (Factory🏭 methods)
    ParseResultArgParser(T& inner) noexcept : inner(types::Ok<T>(inner)) {}
    ParseResultArgParser(E inner) noexcept : inner(types::Err<E>(inner)) {}
    virtual ~ParseResultArgParser() noexcept = default;
    // Get inner result
    types::Result<T, E> as_result() const noexcept { return inner; }
};

template <typename T, typename E>
class ArgResultArgParser : public ParseResultArgParser<T, E> {
   private:
    types::Option<std::string> as_opt_str(
        const types::Option<const char*>& in) {
        types::Option<std::string> res;
        if (in) res = std::string(in.unwrap());
        return res;
    }

   public:
    // Constructors
    ArgResultArgParser(T& inner) noexcept : ParseResultArgParser<T, E>(inner) {}
    ArgResultArgParser(E inner) noexcept : ParseResultArgParser<T, E>(inner) {}
    // Add flags
    ArgResultArgParser<T, E>& flagStr(
        const std::vector<std::string>& aliases, const std::string& desc,
        const types::Option<std::string>& value = {}) noexcept {
        if (this->inner) this->inner.unwrap().flagStr(aliases, desc, value);
        return *this;
    }
    ArgResultArgParser<T, E>& flagBool(
        const std::vector<std::string>& aliases, const std::string& desc,
        const types::Option<bool>& value = {}) noexcept {
        if (this->inner) this->inner.unwrap().flagBool(aliases, desc, value);
        return *this;
    }
    ArgResultArgParser<T, E>& flagInt(
        const std::vector<std::string>& aliases, const std::string& desc,
        const types::Option<int>& value = {}) noexcept {
        if (this->inner) this->inner.unwrap().flagInt(aliases, desc, value);
        return *this;
    }
    ArgResultArgParser<T, E>& flagVec2(
        const std::vector<std::string>& aliases, const std::string& desc,
        const types::Option<const char*>& value = {}) noexcept {
        if (this->inner)
            this->inner.unwrap().flagVec2(aliases, desc, as_opt_str(value));
        return *this;
    }
};

template <typename Error>
class ArgParser {
   private:
    std::unordered_map<std::string, std::string> flag_aliases;
    std::unordered_map<std::string, util::Flag> flag_values;
    // Adds the flags to the map
    inline void _reg(const std::vector<std::string>& aliases, util::Flag flag) {
        flag_values[aliases[0]] = flag;
        for (const std::string& alias : aliases) {
            flag_aliases[alias] = aliases[0];
        }
    }

   public:
    // Add flags
    ArgResultArgParser<ArgParser<Error>&, Error> flagStr(
        const std::vector<std::string>& aliases, const std::string& desc,
        const types::Option<std::string>& value = {}) noexcept {
        _reg(aliases, {util::as_any<std::string>(value), util::FlagType::Str});
        return ArgResultArgParser<ArgParser<Error>&, Error>(*this);
    }
    ArgResultArgParser<ArgParser<Error>&, Error> flagBool(
        const std::vector<std::string>& aliases, const std::string& desc,
        const types::Option<bool>& value = {}) noexcept {
        _reg(aliases, {util::as_any<bool>(value), util::FlagType::Bool});
        return ArgResultArgParser<ArgParser<Error>&, Error>(*this);
    }
    ArgResultArgParser<ArgParser<Error>&, Error> flagInt(
        const std::vector<std::string>& aliases, const std::string& desc,
        const types::Option<int>& value = {}) noexcept {
        _reg(aliases, {util::as_any<int>(value), util::FlagType::Int});
        return ArgResultArgParser<ArgParser<Error>&, Error>(*this);
    }
    ArgResultArgParser<ArgParser<Error>&, Error> flagVec2(
        const std::vector<std::string>& aliases, const std::string& desc,
        const types::Option<std::string>& value = {}) noexcept {
        types::Option<util::vec2> val;
        if (value) {
            types::Result<util::vec2, Error> res =
                util::as_vec2<Error>(value.unwrap(), 'x');
            if (!res)
                return ArgResultArgParser<ArgParser<Error>&, Error>(
                    res.unwrap_err());
            val = res.unwrap();
        }
        _reg(aliases, {util::as_any<util::vec2>(val), util::FlagType::Vec2});
        return ArgResultArgParser<ArgParser<Error>&, Error>(*this);
    }
    ParseResultArgParser<ArgParser<Error>&, Error> parse(

        const int argc, const char** argv) noexcept {
        bool look_ahead = false;
        types::Option<util::FlagParse> current_flag;
        for (int i = 1; i < argc; i++) {
            // Check for arg
            if (!look_ahead) {
                try {
                    util::Flag flag = flag_values.at(flag_aliases.at(i[argv]));
                    current_flag = util::FlagParse{i[argv], flag};
                    if (flag.type == util::FlagType::Bool) {
                        flag_values.at(flag_aliases.at(i[argv])).value =
                            util::as_any<bool>(true);
                    } else {
                        look_ahead = true;
                        if (i + 1 >= argc) {
                            std::stringstream ss;
                            ss << "Not enough arguments at `" << i[argv] << "`";
                            return ParseResultArgParser<ArgParser<Error>&,
                                                        Error>(
                                ss.str().c_str());
                        }
                    }
                } catch (std::exception& e) {
                    std::stringstream ss;
                    ss << "Invalid flag `" << i[argv] << "`";
                    return ParseResultArgParser<ArgParser<Error>&, Error>(
                        ss.str().c_str());
                }
                // Check for flag (⚠️ Warning minimal spaghetti🍝 code)
            } else {
                if (current_flag) {
                    switch (current_flag.unwrap().flag.type) {
                        case util::FlagType::Str:
                            flag_values
                                .at(flag_aliases.at(current_flag.unwrap().key))
                                .value = util::as_any<const char*>(i[argv]);
                        case util::FlagType::Int:
                            flag_values
                                .at(flag_aliases.at(current_flag.unwrap().key))
                                .value = util::as_any<int>(std::stoi(i[argv]));
                            break;
                        case util::FlagType::Vec2: {
                            auto res = util::as_vec2<Error>(i[argv], 'x');
                            if (!res)
                                return ParseResultArgParser<ArgParser<Error>&,
                                                            Error>(
                                    res.unwrap_err());
                            flag_values
                                .at(flag_aliases.at(current_flag.unwrap().key))
                                .value = util::as_any<util::vec2>(res.unwrap());
                        } break;
                        default:
                            break;
                    }
                }
                look_ahead = false;
            }
        }
        return ParseResultArgParser<ArgParser<Error>&, Error>(*this);
    }
    template <util::FlagType T>
    types::Option<typename util::FlagTypeMap<T>::type> get(
        const std::string& key) {
        try {
            auto any_flag = flag_values.at(flag_aliases.at(key)).value;
            return util::from_any<typename util::FlagTypeMap<T>::type>(
                any_flag);
        } catch (...) {
            return {};
        }
    };
};

}  // namespace util
#endif  // !PARSER_H
