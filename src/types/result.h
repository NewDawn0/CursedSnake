// @AUTHORS: NewDawn0
// @DATE: 2023-11-30
// @FILE: types/result.h
// @TYPE: Header only
// @DESC: Argument parser

// Header guard
#ifndef RESULT_H
#define RESULT_H

// Includes
#include <memory>
#include <stdexcept>

// Namespaces
namespace types {

// Result class (Inspired by Rust) 
template<typename T, typename E>
class Result {
protected:
    T* val;                                           // Ok value ptr
    E* err;                                           // Err value ptr
    // Constructor : [Internal]
    Result(T* val, E* err)                            // The generic internal constructor
        noexcept : val(val), err(err) {}              // Called by the Ok & Err static methods
public:
    // Constructors & Destructors
    ~Result() noexcept {                              // Generic dectructor:
        delete val;                                   // Simply deletes the values
        delete err;
    }
    Result(const Result& old) noexcept {              // Copy constructor:
        val = old.val ? new T(*old.val) : nullptr;    // Copies the values from the old
        err = old.err ? new E(*old.err) : nullptr;    // Result to the new Result
    }
    Result& operator=(const Result& old) noexcept {   // Asignement operator impl for
        if (this != &old) {                           // the copy constructor:
            delete val;                               // Deletes the values and replaces them
            delete err;
            val = old.val ? new T(*old.val) : nullptr;// with new ptrs of the old values
            err = old.err ? new E(*old.err) : nullptr;
        }
        return *this;
    }
    Result(Result&& old) noexcept                     // Move constructor:
        : val(old.val), err(old.err) {                // Moves the the old values
        old.val = nullptr;                            // to the new Result
        old.err = nullptr;
    }
    Result& operator=(Result&& old) noexcept {        // Asignement operator impl for
        if (this != &old) {                           // the move constructor:
            delete val;                               // Simply deletes the values
            delete err;                               // with the moved ptrs of the old
            val = old.val;                            // Result and then sets the old values to NULL
            err = old.err;
            old.val = nullptr;
            old.err = nullptr;
        }
        return *this;
    }
    static Result Ok(T val) noexcept {                // Generic Ok static initalizer method:
        return Result(new T(val), nullptr);           // Intializes a Result with <value, NULL>
    }
    static Result Err(E err) noexcept {               // Generic Err static initalizer method:
        return Result(nullptr, new E(err));           // Intializes a Result with <NULL, error>
    }

    // Check if Result has a value/error
    bool isOk() const noexcept { return !!val; }
    bool isErr() const noexcept { return !!err; }

    // Retrieve Ok/Err values
    T getVal() const {                                // Gets value otherwise throws an exception
        if (!val) throw std::logic_error("No value in result");
        return *val;
    }
    E getErr() const {                                // Gets error otherwise throws an exception
        if (!err) throw std::logic_error("No error in result");
        return *err;
    }
};

}       // !namespace types
#endif  // !RESULT_H
