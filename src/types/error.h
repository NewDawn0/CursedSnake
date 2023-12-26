// @AUTHORS: NewDawn0
// @DATE: 2023-12-06
// @FILE: types/error.h
// @TYPE: Header only
// @DESC: An Error class for types

// Header guard
#ifndef ERROR_H
#define ERROR_H

// Includes
#include <string>

// Namespaces
namespace types {

// The Error base class
struct Error {
   private:
    const char* msg;

   public:
    Error() : msg("Generic error") {}
    Error(const char* msg) : msg(msg) {}
    virtual ~Error() = default;
    virtual std::string what() const { return msg; }
};

}  // namespace types
#endif  // !ERROR_H
