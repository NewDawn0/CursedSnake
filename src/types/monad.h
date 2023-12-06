// @AUTHORS: NewDawn0
// @DATE: 2023-11-30
// @FILE: types/monad.h
// @TYPE: Header only
// @DESC: Ads monadic types 

// Header guard
#ifndef MONAD_H
#define MONAD_H

// Includes
#include "result.h"
#include <type_traits>
#include <utility>

// Namespaces
namespace types {

// MonadicResult class is a Monad combined combined with the Result
template<typename T, typename E>
class MonadicResult : public Result<T, E> {                           // Inherit from Result<T,E>
private:
    using Result<T, E>::Result;                                       // Bring Result constructors into scope
    MonadicResult(T* val, E* err)                                     // Generic MonadicResult constructor
        noexcept : Result<T, E>(val, err) {}
public:
    MonadicResult(const MonadicResult&) noexcept = default;           // Use Result copy constructor
    MonadicResult(MonadicResult&&) noexcept = default;                // Use Result move constructor
    MonadicResult& operator=(const MonadicResult&) noexcept = default;// Use Result asignment copy constructor
    MonadicResult& operator=(MonadicResult&&) noexcept = default;     // Use Result asignment move constructor
    static MonadicResult Ok(T val) noexcept {                         // Generic Ok static initalizer method:
        return MonadicResult(new T(val), nullptr);                    // Intializes a Result with <value, NULL>
    }
    static MonadicResult Err(E err) noexcept {                        // Generic Err static initalizer method:
        return MonadicResult(nullptr, new E(err));                    // Intializes a Result with <NULL, error>
    }

    // Bind & Bindm funcs
    template<typename Fn, typename... Args>                           // Bind a function with its arguments
    MonadicResult& bind(Fn f, Args&&... args) const noexcept {        // and execute it with the inner monad type
        using RT = std::invoke_result_t<Fn, T, Args...>;              // @NOTE: First type in function automatically used as monad value
        static_assert(std::is_convertible<RT, T>::value, "The result of the function f must be convertible to T");
        if (this->isOk()) {
            try {
                auto res = f(this->getVal(), std::forward<Args>(args)...);
                *this = MonadicResult::Ok(static_cast<T>(res));
            } catch (const E& err) {
                *this = MonadicResult::Err(err);
            } catch (...) {
                *this = MonadicResult::Err("An unexpecte error occured");
            }
        }
        return *this;
    }
    
    template<typename Fn, typename... Args, typename enable = std::is_class<T>>
    MonadicResult& bindm(Fn f, Args&&... args) const noexcept {       // Binds a method of a class to the monadic
        if (this->isOk()) {                                           // type and executes the method of the type
            try {                                                     // @NOTE: Only use it when <T> is a class with methods
                (this->val->*f)(std::forward<Args>(args)...);
            } catch (const E& err) {
                *this = MonadicResult::Err(err);
            } catch (...) {
                *this = MonadicResult::Err("An unexpecte error occured");
            }
        }
        return *this;
    }
};

}       // !namespace types
#endif  // !MONAD_H
