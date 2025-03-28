/**
 * @file DataLogger.cpp
 * @brief Implements the DataLogger class methods for logging sensor data.
 *
 * This file contains the implementation of the DataLogger class, which logs sensor data (e.g., heart rate,
 * blood pressure, cholesterol, ECG) to a file. It handles file operations and error reporting using the
 * ErrorHandling class.
 *
 * @note The log file is opened in append mode. If the file is empty, a header row is written.
 *
 * @author Sarah Soliman
 */

#include "DataLogger.h"

/**
 * @brief Constructs a new DataLogger object.
 *
 * Initializes the DataLogger with the provided log file name. Opens the file in append mode.
 * If the file fails to open, an error message is logged. If the file is empty, a header line is written.
 *
 * @param fileName The name (and path) of the log file.
 */

DataLogger :: DataLogger(const std ::string& fileName){
    filePath = fileName;
    fileStream.open(filePath, std ::ios::app); 

    if(!fileStream.is_open()){
        ErrorHandling::logErrorMessage("Failed to open the log file"+ filePath);
        return; 
    }

    fileStream.seekp(0, std::ios::end);
    if(fileStream.tellp() == 0){
        fileStream <<"TImestamp, HeartRate,SysBP,DiaBP,Cholesterol,ECG\n";
    }
}

/**
 * @brief Destructor for the DataLogger class.
 *
 * Closes the log file if it is open.
 */

DataLogger :: ~DataLogger(){
    if(fileStream.is_open()) {
        fileStream.close();
    }
}

/**
 * @brief Retrieves the current timestamp as a formatted string.
 *
 * Obtains the current system time and formats it according to the pattern "%y-%m-%d %H:%M:%S".
 *
 * @return std::string The formatted current timestamp.
 */

std :: string DataLogger ::getCurrentTimestamp(){
    auto now = std:: chrono:: system_clock::now();
    std:: time_t now_time = std ::chrono ::system_clock::to_time_t(now);
    char buffer [80];
    strftime(buffer,sizeof(buffer), "%y-%m-%d %H:%M:%S", localtime(&now_time));
    return std:: string(buffer);
}

/**
 * @brief Logs sensor data to the log file.
 *
 * Writes a line of sensor data, including the timestamp and various sensor readings,
 * to the log file. The data is separated by commas. If the log file is not open, an error is logged.
 *
 * @param timestamp The timestamp associated with the sensor data.
 * @param heartRate The heart rate reading.
 * @param sysBP The systolic blood pressure reading.
 * @param diasBP The diastolic blood pressure reading.
 * @param cholesterol The cholesterol level reading.
 * @param ecg The ECG reading.
 */

void DataLogger :: logData(const std:: string& timestamp, double heartRate, double sysBP, double diasBP, double cholesterol, double ecg){

    try{
        if(fileStream.is_open()){
            fileStream <<timestamp << ", " << heartRate << " , " << sysBP << ", " << diasBP << ", " << cholesterol << ", "<< ecg << "\n";
            fileStream.flush(); // the data will be updated/ written 

        }else {
            ErrorHandling::logErrorMessage("The log file is not open.");
        }

    } catch (const std :: exception& e){
        ErrorHandling:: handleException(e);
    }
}
