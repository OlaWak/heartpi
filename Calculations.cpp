/**
 * @file Calculations.cpp
 * @brief Contains the implementation of the heart health assessment function.
 *
 * This file implements the function to assess heart health by calculating a risk score
 * based on various parameters from a FamilyHealth object, and simulates sensor readings
 * accordingly.
 *
 * @author Sena Debian and Yosra Alim
 */

 #include "Calculations.h"
 #include <iostream>
 #include "RandomNumberGenerator.h"
 #include "FamilyHealth.h"
 
 /**
  * @brief Assesses heart health based on family health parameters and simulates sensor readings.
  *
  * This function calculates a risk score based on various factors extracted from the 
  * FamilyHealth object. It evaluates the age group, gender, sleep hours, exercise frequency,
  * family disease history, diet type, and smoking status. The risk score is then used to 
  * simulate sensor readings for heart rate, systolic blood pressure, diastolic blood pressure,
  * cholesterol, and ECG using randomly generated values.
  *
  * The thresholds for risk score are used as follows:
  * - Low risk: risk score < 10
  * - Moderate risk: risk score >= 10 and < 18
  * - High risk: risk score >= 18
  *
  * @param[in] family A reference to a FamilyHealth object containing health-related information.
  * @param[out] heartRate A reference to a double where the simulated heart rate will be stored.
  * @param[out] sysBP A reference to a double where the simulated systolic blood pressure will be stored.
  * @param[out] diasBP A reference to a double where the simulated diastolic blood pressure will be stored.
  * @param[out] cholesterol A reference to a double where the simulated cholesterol level will be stored.
  * @param[out] ecg A reference to a double where the simulated ECG reading will be stored.
  *
  * @return A string describing the assessed risk level for heart disease.
  *
  * @note The simulation ranges vary depending on the computed risk score.
  */
 std::string assessHeartHealth(const FamilyHealth& family, double& heartRate,
     double& sysBP, double& diasBP,
     double& cholesterol, double& ecg)
 {
     int riskScore = 0;
     
     // 1) Age-based
     if (family.getAgeGroup() == 1 || family.getAgeGroup() == 2)
         riskScore += 1;
     else if (family.getAgeGroup() == 3 || family.getAgeGroup() == 4)
         riskScore += 2;
     else
         riskScore += 3;
     
     // 2) Gender-based: interpret 'false' as Female and 'true' as Male.
     if (!family.getGender())  // false => Female
     {
         if (family.getAgeGroup() <= 3)
             riskScore += 1;
         else
             riskScore += 3;
     }
     else // Male
     {
         if (family.getAgeGroup() <= 3)
             riskScore += 2;
         else
             riskScore += 3;
     }
     
     // 3) Sleep Hours: e.g. 1 => <4 hrs => +3, 2 => 4-5 hrs => +2, 3 => 6-7 hrs => +1, 4 => 7-8 hrs => +1, 5 => >8 hrs => +3
     if (family.getSleepHours() == 1 || family.getSleepHours() == 5)
         riskScore += 3;
     else if (family.getSleepHours() == 2)
         riskScore += 2;
     else
         riskScore += 1;
     
     // 4) Exercise Frequency: e.g. 1 => never => +3, 2 => 1-2/wk => +2, else => +1
     if (family.getExerciseFrequency() == 1)
         riskScore += 3;
     else if (family.getExerciseFrequency() == 2)
         riskScore += 2;
     else
         riskScore += 1;
     
     // 5) Family disease(s): If the user has multiple diseases, each adds to the score.
     if (family.hasFamilyDisease(0)) riskScore += 2; // Heart disease
     if (family.hasFamilyDisease(1)) riskScore += 1; // Diabetes
     if (family.hasFamilyDisease(2)) riskScore += 2; // High cholesterol
     if (family.hasFamilyDisease(3)) riskScore += 2; // High blood pressure
     
     // 6) Diet type:
     // 1 = High Protein, 2 = Low Carb, 3 = Vegetarian, 4 = Western, 5 = Vegan, 6 = Balanced
     int dt = family.getDietType();
     if (dt == 4) // Western
         riskScore += 3;
     else if (dt == 1 || dt == 2 || dt == 3 || dt == 6)
         riskScore += 1;
     else if (dt == 5)
         riskScore += 2;
     
     // 7) Smoking
     if (family.getIsSmoker())
         riskScore += 3;
     else
         riskScore += 1;
     
     // Print riskScore for debugging
     std::cout << "Risk Score: " << riskScore << std::endl;
     
     // Simulate sensor readings based on risk score
     if (riskScore < 10) {
         // Low risk
         heartRate   = RandomNumberGenerator(60,  80).generate();
         sysBP       = RandomNumberGenerator(110, 120).generate();
         diasBP      = RandomNumberGenerator(70, 80).generate();
         cholesterol = RandomNumberGenerator(150, 200).generate();
         ecg         = RandomNumberGenerator(0.05, 0.15).generate();
     } else if (riskScore < 18) {
         // Moderate risk
         heartRate   = RandomNumberGenerator(80, 95).generate();
         sysBP       = RandomNumberGenerator(120, 135).generate();
         diasBP      = RandomNumberGenerator(80, 90).generate();
         cholesterol = RandomNumberGenerator(200, 240).generate();
         ecg         = RandomNumberGenerator(0.02, 0.18).generate();
     } else {
         // High risk
         heartRate   = RandomNumberGenerator(95, 120).generate();
         sysBP       = RandomNumberGenerator(135, 160).generate();
         diasBP      = RandomNumberGenerator(90, 110).generate();
         cholesterol = RandomNumberGenerator(240, 300).generate();
         ecg         = RandomNumberGenerator(-0.1, 0.3).generate();
     }
     
     // Final risk assessment
     if (riskScore >= 18) 
         return "High risk of heart disease.";
     else if (riskScore >= 10)
         return "Moderate risk of heart disease.";
     else
         return "Low risk of heart disease. You are healthy!";
 }
 