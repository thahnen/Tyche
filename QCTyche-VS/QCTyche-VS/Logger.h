#pragma once

#include <iostream>
#include <string>


/// The critical level of the message
typedef enum {
    INFO = 0,		// The message logged is an info
    WARNING,        // The message logged is a warning
    LERROR,         // The message logged is an error (its called LERROR because ERROR was already defined somewhere else!)
} LLEVEL;


/**
 *  Logs a message to console, identified by logging level
 *
 *  @param level            the critical level of the
 *  @param msg              the message which shall be logged
 */
void log(LLEVEL level, std::string msg);
