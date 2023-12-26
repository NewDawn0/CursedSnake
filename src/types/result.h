// @AUTHORS: NewDawn0
// @DATE: 2023-12-06
// @FILE: types/result.h
// @TYPE: Header only
// @DESC: Rust's Result data type

// Header guard
#ifndef RESULT_H
#define RESULT_H

// Includes
#include <stdexcept>
#include <string>
#include <variant>

// Namespaces
namespace types {

// Wrapper for Ok value
template <typename T>
struct Ok {
    T inner;
    explicit Ok(const T& val) noexcept : inner(val) {}
};

// Wrapper for Err value
template <typename E>
struct Err {
    E inner;
    explicit Err(const E& val) noexcept : inner(val) {}
};

// Result class (Inspired by Rust)
template <typename T, typename E>
class Result {
   private:
    std::variant<Ok<T>, Err<E>> inner;

   public:
    // Constructors : explicit
    Result(const Ok<T>& val) noexcept : inner(val) {}
    Result(const Err<E>& err) noexcept : inner(err) {}
    // Constructors : implicit
    Result(const T& val) noexcept : inner(Ok<T>{val}) {}
    Result(const E& err) noexcept : inner(Err<E>{err}) {}
    explicit operator bool() const noexcept {
        return std::holds_alternative<Ok<T>>(inner);
    }
    T expect(const char* msg) const {
        if (!(*this)) throw std::runtime_error(msg);
        return std::get<Ok<T>>(inner).inner;
    }
    T unwrap() const {
        if (!(*this)) throw std::runtime_error("Called unwrap on an error");
        return std::get<Ok<T>>(inner).inner;
    }
    E unwrap_err() const {
        if (*this) throw std::runtime_error("Called unwrap_err on a result");
        return std::get<Err<E>>(inner).inner;
    }
};

}  // namespace types
#endif  // !RESULT_H
