// @AUTHORS: NewDawn0
// @DATE: 2023-12-06
// @FILE: types/option.h
// @TYPE: Header only
// @DESC: Rust's Option data type

// Header guard
#ifndef OPTION_H
#define OPTION_H

// Includes
#include <optional>
#include <stdexcept>

// Namespaces
namespace types {

// Wrapper for Some value
template <typename T>
struct Some {
    T inner;
    explicit Some(const T& val) noexcept : inner(val) {}
};

// Wrapper for None value
struct None {
    explicit None() noexcept {}
};

// Option type insipred by Rust🦀
template <typename T>
class Option {
   private:
    std::optional<Some<T>> inner;

   public:
    // Constructors
    Option() noexcept : inner(std::nullopt) {}
    Option(const T& val) noexcept : inner(Some<T>(val)) {}
    Option(const None) noexcept : inner(std::nullopt) {}
    Option(const Some<T>& val) noexcept : inner(val) {}
    explicit operator bool() const noexcept { return inner.has_value(); }
    T expect(const char* msg) const {
        if (!(*this)) throw std::runtime_error(msg);
        auto ptr = *inner;
        return inner->inner;
    }
    T unwrap() const {
        if (!(*this)) throw std::runtime_error("Called unwrap on none option");
        return inner->inner;
    }
};

}  // namespace types
#endif  // !OPTION_H
