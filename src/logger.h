// @AUTHORS: NewDawn0
// @DATE: 2023-11-25
// @FILE: logger.h
// @TYPE: Header only
// @DESC: Logger

// Header guard
#ifndef LOGGER_H
#define LOGGER_H

// Includes
#include <stdexcept>
#include <string>
#include <iostream>
#include <strstream>

// Namespaces
namespace logger {

// Log level type
enum LogLevel {
    Disabled = 0,
    Error = 1,
    Debug = 2,
    All = 3,
};

// Logger config
struct LoggerCfg {
    struct _colours {
        std::string info = "\x1b[32;1m";
        std::string error = "\x1b[31;1m";
        std::string debug = "\x1b[33;1m";
        std::string unset = "\x1b[0m";
    } colours;
    bool enable_intrinsics = true;
    bool enable_colours = true;
    LogLevel logLvl = LogLevel::Debug;
};

// Custom logger class
class Logger {
private:
    LoggerCfg config;
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }
    inline void configure(LoggerCfg cfg) {
        config = cfg;
    }
    inline void logma(std::string msg, LogLevel providedLvl, const char* file, int line) {
        if (providedLvl != LogLevel::Disabled && providedLvl <= config.logLvl) {
            std::string col;       // Set colour
            std::string typemsg;   // Set typemsg
            switch (providedLvl) {
                case LogLevel::All:
                    col = config.colours.info;
                    typemsg = "INFO ";
                    break;
                case LogLevel::Debug:
                    col = config.colours.debug;
                    typemsg = "DEBUG ";
                    break;
                case LogLevel::Error:
                    col = config.colours.error;
                    typemsg = "ERROR ";
                    break;
                default:
                    break;
            };
            switch (providedLvl) {
                case LogLevel::All: {
                    std::cout
                        << ([this, &col](){ return config.enable_colours ? col : ""; })()
                        << typemsg
                        << ([this, &file, &line](){ if (config.enable_intrinsics) { std::ostrstream oss; oss << "<[" << file << ":" << line << "]> "; return oss.str(); } else { return (char*)""; }})()
                        << ([this](){ return config.enable_colours ? config.colours.unset: ""; })()
                        << ": "+msg
                        << std::endl;
                    };
                    break;
                default: {
                    std::cerr
                        << ([this, &col](){ return config.enable_colours ? col : ""; })()
                        << typemsg
                        << ([this, &file, &line](){ if (config.enable_intrinsics) { std::ostrstream oss; oss << "<[" << file << ":" << line << "]> "; return oss.str(); } else { return (char*)""; }})()
                        << ([this](){ return config.enable_colours ? config.colours.unset: ""; })()
                        << ": "+msg
                        << std::endl;
                    };
                    break;
            }
        }
    };
};

// Macros
#define LOG_INFO(msg) Logger::getInstance().logma(msg, logger::LogLevel::All, __FILE__, __LINE__) 
#define LOG_DEBUG(msg) Logger::getInstance().logma(msg, logger::LogLevel::Debug, __FILE__, __LINE__) 
#define LOG_ERROR(msg) Logger::getInstance().logma(msg, logger::LogLevel::Error, __FILE__, __LINE__) 

// Global instances
Logger logger;

}       // !namespace logger
#endif  // !LOGGER_H
