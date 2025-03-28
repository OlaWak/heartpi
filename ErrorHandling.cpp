/**
 * @file ErrorHandling.cpp
 * @brief Implements error handling utilities for logging error messages and exceptions.
 *
 * This file defines functions to obtain the current timestamp, log error messages to both
 * standard error and a log file ("Error_log.txt"), and handle exceptions by logging them.
 *
 * @author Sarah Soliman
 */

#include "ErrorHandling.h"

/**
 * @brief Retrieves the current timestamp as a formatted string.
 *
 * Obtains the current system time and converts it into a human-readable string format
 * using std::ctime.
 *
 * @return std::string The current timestamp as a string.
 */
std :: string ErrorHandling :: getCurrentTimestamp (){
    auto now = std:: chrono :: system_clock :: now();
    std:: time_t now_time = std:: chrono :: system_clock :: to_time_t(now);
     
    return std :: ctime(&now_time);
}

/**
 * @brief Logs an error message to standard error and a log file.
 *
 * This function writes the provided error message to the standard error stream,
 * prefixed with an "[ERROR!]" tag and the current timestamp. It also appends the
 * error message to "Error_log.txt" if the file can be opened.
 *
 * @param message The error message to log.
 */


/**
 * @brief Handles exceptions by logging the exception message.
 *
 * Constructs an error message by prepending "Exception caught: " to the exception's
 * what() message, then logs the complete error message using logErrorMessage().
 *
 * @param e The exception object that was caught.
 */
void ErrorHandling :: logErrorMessage(const std:: string& message){
    std :: cerr << "[ERROR!]" << getCurrentTimestamp() << ":" << message << std :: endl;

    std:: ofstream logFile("Error_log.txt", std :: ios:: app);
    if(logFile.is_open()){
        logFile <<"[ERROR!]" << getCurrentTimestamp() << ":" << message << std :: endl;
        logFile.close();
    }

}

void ErrorHandling :: handleException(const std ::exception& e){
    logErrorMessage(std::string("Exception caught: ") + e.what());
}
