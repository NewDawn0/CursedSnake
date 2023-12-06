// @AUTHORS: NewDawn0
// @DATE: 2023-11-30
// @FILE: types/error.h
// @TYPE: Header only
// @DESC: Adds an error type

// Header guard
#ifndef ERROR_H
#define ERROR_H

// Includes
#include <string>
#include <sstream>
#include <strstream>

// Namespaces
namespace types {

// Error class
class Error {
private:
    const char* msg;                                                                // Store the message
    const char* file;                                                               // store the file
    const int line;                                                                 // Store the line
public:
    Error(const char* msg, const char* file = __FILE__, const int line = __LINE__)  // Generic error constructor
        noexcept : msg(std::move(msg)), file(file), line(line) {}                   // with a message a file and a line
    
    std::string str() const noexcept {                                              // Stringify the error
        std::ostrstream oss;                                                        // Using the formatting:
        oss << "ERROR <[" << file << ":" << line << "]> : " << msg;                 // [file:line] msg
        return oss.str();
    }
};

// Error definition
#define Error(msg) Error(msg, __FILE__, __LINE__)                                   // Constructor from str with automatic line and file

}       // !namespace types
#endif  // !ERROR_H
