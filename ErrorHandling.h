// This program displays a centralized errorhandling class that finds log errors from diferent modules RandomNumberGeneration and Calculations 
#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

/**
 * @file ErrorHandling.h
 * @brief Declaration of the ErrorHandling class.
 *
 * This file declares a centralized error handling class that is used to log error messages and handle exceptions
 * across different modules, such as RandomNumberGeneration and Calculations.
 *
 * @note The class provides static methods to log error messages to both standard error and a file.
 *
 * @author Sarah Soliman
 */

#include <exception> 
#include <string> 
#include <iostream> 
#include <fstream>
#include <chrono> 
#include <ctime>

/**
 * @class ErrorHandling
 * @brief Provides utilities for error logging and exception handling.
 *
 * The ErrorHandling class centralizes the logging of error messages and handling of exceptions. It includes
 * methods to log error messages with a timestamp and to handle exceptions by logging the exception message.
 */

class ErrorHandling{
public: 

    /**
     * @brief Logs an error message.
     *
     * Writes the provided error message to the standard error stream and appends it to a log file.
     *
     * @param message The error message to be logged.
     */

    static void logErrorMessage(const std:: string& message);
    
    /**
     * @brief Handles exceptions by logging the exception message.
     *
     * This function logs the exception message, prefixed with "Exception caught: ", using the logErrorMessage method.
     *
     * @param e The exception object that was caught.
     */
    static void handleException(const std::exception& e);

private: 

     /**
     * @brief Retrieves the current timestamp.
     *
     * Obtains the current system time and formats it as a human-readable string.
     *
     * @return std::string The current timestamp.
     */
    
    static std :: string getCurrentTimestamp();

};

#endif 
