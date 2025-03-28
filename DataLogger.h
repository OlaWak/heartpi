/**
 * @file DataLogger.h
 * @brief Declaration of the DataLogger class.
 *
 * This file declares the DataLogger class, which is responsible for logging sensor data such as heart rate,
 * blood pressure, cholesterol, and ECG readings to a CSV file. The class includes functionality to obtain
 * the current timestamp and write data to the file.
 *
 * @author Sara Soliman
 */


// This displays the DataLogger system where it logs the heart rate and other information with the timestamps in CSV file. 
#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <exception> 
#include <string> 
#include <iostream> 
#include <fstream>
#include <chrono> 
#include <ctime>
#include "ErrorHandling.h"


/**
 * @class DataLogger
 * @brief Logs sensor data to a CSV file.
 *
 * The DataLogger class handles file operations for logging sensor data. It writes the data along with
 * timestamps to a CSV file, ensuring that data is appended correctly and the file is properly closed.
 */

class DataLogger{  

private: 
    std::string filePath; 
    std:: ofstream fileStream;  //fileStream is being declared 

    
    /**
     * @brief Retrieves the current timestamp.
     *
     * Obtains and formats the current system time to be used as a timestamp for logged data.
     *
     * @return std::string The current timestamp formatted as a string.
     */
    std::string getCurrentTimestamp(); // to get the current timestamp

public :
    
    /**
     * @brief Constructs a new DataLogger object.
     *
     * Initializes the DataLogger with the specified CSV file name. Opens the file in append mode.
     *
     * @param filename The name (and path) of the CSV log file.
     */

    DataLogger(const std:: string& filename); // constructor for the  CSV file 
    
    /**
     * @brief Destructor for the DataLogger class.
     *
     * Ensures that the file stream is properly closed when the DataLogger object is destroyed.
     */

    ~DataLogger(); // closes the file 
    // CSV file updatated with the information and timestamp

    /**
     * @brief Logs sensor data to the CSV file.
     *
     * Writes the provided timestamp and sensor readings (heart rate, systolic and diastolic blood pressure,
     * cholesterol, and ECG) as a new line in the CSV file.
     *
     * @param timestamp The timestamp for the logged data.
     * @param heartRate The heart rate reading.
     * @param sysBP The systolic blood pressure reading.
     * @param diasBP The diastolic blood pressure reading.
     * @param cholesterol The cholesterol level reading.
     * @param ecg The ECG reading.
     */
    void logData(const std :: string& timestamp, double heartRate, double sysBP, double diasBP, double cholesterol, double ecg);  

};

#endif
