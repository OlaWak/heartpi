/**
 * @file Calculations.h
 * @brief Header file for heart health assessment functionality.
 *
 * This file declares the function prototype for assessing heart health by computing a risk
 * score based on a FamilyHealth object and simulating sensor readings such as heart rate,
 * blood pressure, cholesterol, and ECG.
 *
 * @author Sena Debian and Yosra Alim
 */

#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include "FamilyHealth.h"
#include <string>

/**
 * @brief Assesses heart health and simulates sensor readings.
 *
 * This function computes a risk score based on various parameters contained in the FamilyHealth object,
 * including age, gender, sleep hours, exercise frequency, family disease history, diet type, and smoking status.
 * It then simulates sensor readings for heart rate, systolic blood pressure, diastolic blood pressure, cholesterol,
 * and ECG using randomly generated values based on the risk score thresholds.
 *
 * The thresholds for risk score are used as follows:
 * - Low risk: risk score < 10
 * - Moderate risk: risk score >= 10 and < 18
 * - High risk: risk score >= 18
 *
 * @param[in] family A reference to a FamilyHealth object containing the user's health data.
 * @param[out] heartRate A reference to a double where the simulated heart rate will be stored.
 * @param[out] sysBP A reference to a double where the simulated systolic blood pressure will be stored.
 * @param[out] diasBP A reference to a double where the simulated diastolic blood pressure will be stored.
 * @param[out] cholesterol A reference to a double where the simulated cholesterol level will be stored.
 * @param[out] ecg A reference to a double where the simulated ECG reading will be stored.
 *
 * @return A string message representing the assessed risk of heart disease.
 *
 * @note The simulated sensor readings are based on predefined ranges corresponding to low, moderate,
 * and high risk scores.
 */
std::string assessHeartHealth(const FamilyHealth& family, double& heartRate, double& sysBP, double& diasBP, double& cholesterol, double& ecg);

#endif // CALCULATIONS_H
