// @AUTHORS: NewDawn0
// @DATE: 2023-11-25
// @FILE: types.h
// @TYPE: Header only
// @DESC: Utilility types

// Header guard
#ifndef TYPES_H
#define TYPES_H

// Includes
#include <strstream>
#include <optional>
#include <string>

// Namespaces
namespace types {

// constexprs
constexpr auto None = std::nullopt;

// Custom error class 
class Error {
public:
    struct _data {
        const char* file =  __FILE__;
        int line  = __LINE__;
        std::string msg;
    } data;
    std::string str() {
        std::ostrstream oss;
        oss << "ERROR <[" << data.file << ":" << data.line << "]> : " << data.msg;
        return oss.str();
    }
    Error() { data.msg = ""; }
    Error(std::string msg) {
        data.msg = msg;
    };
};

// Rust-like result type
template<typename T, typename E>
class Result {
private:
    bool is_ok;
    T value;
    E err;
    Result(bool is_ok, T value, E err) : is_ok(is_ok), value(value), err(err) {}
public:
    // Constructors for OK and Error results
    static Result<T, E> Ok(T val) {
        return Result<T, E>(true, val, E());
    }
    static Result<T, E> Err(std::string msg) {
        return Result<T, E>(false, T(), E(msg));
    }
    bool isOk() const { return is_ok; }
    bool isErr() const { return !is_ok; }
    T getValue() const { return value; }
    Error getErr() const { return err; }
};

}       // !namespace types
#endif  // !TYPES_H
