// @AUTHORS: NewDawn0
// @DATE: 2023-12-06
// @FILE: types/option.h
// @TYPE: Header only
// @DESC: Argument parser

// Header guard
#ifndef OPTION_H
#define OPTION_H 

// Includes
#include <memory>
#include <stdexcept>

// Namespaces
namespace types {

// Result class (Inspired by Rust) 
template<typename T>
class Option {
private:
    T* val;                                           // Some value ptr
    // Constructor : [Internal]
    Option(T* val)                                    // The generic internal constructor
        noexcept : val(val) {}                        // Called by the Some static methods
public:
    // Constructors & Destructors
    ~Option() noexcept {                              // Generic dectructor:
        delete val;                                   // Simply deletes the values
    }
    Option(const Option& old) noexcept {              // Copy constructor:
        val = old.val ? new T(*old.val) : nullptr;    // Copies the values from the old
    }
    Option& operator=(const Option& old) noexcept {   // Asignement operator impl for
        if (this != &old) {                           // the copy constructor:
            delete val;                               // Deletes the values and replaces them
            val = old.val ? new T(*old.val) : nullptr;// with new ptrs of the old values
        }
        return *this;
    }
    Option(Option&& old) noexcept                     // Move constructor:
        : val(old.val) {                              // Moves the the old values
        old.val = nullptr;                            // to the new Result
    }
    Option& operator=(Option&& old) noexcept {        // Asignement operator impl for
        if (this != &old) {                           // the move constructor:
            delete val;                               // Simply deletes the values
            val = old.val;                            // then sets the old values to NULL
            old.val = nullptr;
        }
        return *this;
    }
    static Option Some(T val) noexcept {              // Generic Ok static initalizer method:
        return Option(new T(val));                    // Intializes a Result with <value, NULL>
    }
    static Option None() noexcept {                   // Generic Err static initalizer method:
        return Option(nullptr);                       // Intializes a Result with <NULL, error>
    }

    // Check if Option has a value/error
    bool isSome() const noexcept { return !!val; }
    bool isNone() const noexcept { return !val; }

    // Retrieve Ok/Err values
    T get() const {                                   // Gets value otherwise throws an exception
        if (!val) throw std::logic_error("No value in option");
        return *val;
    }
};

}       // !namespace types
#endif  // !OPTION_H
