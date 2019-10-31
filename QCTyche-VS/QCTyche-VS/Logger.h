#pragma once

#include <iostream>
#include <string>


/// The critical level of the message
typedef enum {
    INFO,           // The message logged is an info
    WARNING,        // The message logged is a warning
    ERROR,          // The message logged is an error
} LLEVEL;


/**
 *  Logs a message to console, identified by logging level
 *
 *  @param level            the critical level of the
 *  @param msg              the message which shall be logged
 */
void log(LLEVEL level, std::string& msg) {
    std::string out;
    switch (level) {
    case INFO:
        out = "[INFO] ";
        break;
    case WARNING:
        out = "[WARNING] ";
        break;
    case ERROR:
        out = "[ERROR] ";
        break;
    default:
        // The critical level is not implemented yet
        std::cerr << "[ERROR] Logger critical level not implemented yet!" << std::endl;
        return;
    }

    // Append timestamp to logging message
    // Test if \n is in the message, then append two \t after it!

    out += msg;

    // Output message to stdout (easiest access for user)
    std::cout << out << std::endl;
}
