/**
 * @file FamilyHealth.cpp
 * @brief Implements the FamilyHealth class methods for gathering and accessing family health data.
 *
 * This file defines the constructor, interactive information gathering, and getter functions
 * for the FamilyHealth class. It is used to collect and provide health-related data, including
 * age group, gender, sleep habits, exercise frequency, dietary preferences, smoking status, and
 * family disease history.
 * 
 * @author Sena Debian and Yosra Alim
 */

 #include "FamilyHealth.h"
 #include <iostream>
 
 /**
  * @brief Constructs a new FamilyHealth object.
  *
  * Initializes the familyDiseases vector with predefined disease names and sets the familyDiseasesHistory
  * vector to default values (false).
  */
 FamilyHealth::FamilyHealth() 
     : familyDiseases(4), familyDiseasesHistory(4, false) 
 {
     familyDiseases[0] = "Heart attack or coronary artery disease";
     familyDiseases[1] = "Diabetes (Type 2)";
     familyDiseases[2] = "High cholesterol";
     familyDiseases[3] = "High blood pressure";
 }
 
 /**
  * @brief Gathers health-related information from the user.
  *
  * Prompts the user to enter various health parameters including age, gender, sleep hours,
  * exercise frequency, family disease history, diet type, and smoking status.
  * The responses are stored in the object's private members.
  */
 void FamilyHealth::gatherInfo() {
     std::cout << "Q1) What is your age?\n1. 18 - 24\n2. 25 - 34\n3. 35 - 44\n4. 45 - 54\n5. 55 - 64\n6. 65+\n";
     std::cin >> ageGroup;
     std::cin.ignore();
 
     std::cout << "Q2) What is your gender assigned at birth?\n1. Female\n2. Male\n";
     int recordedGender;
     std::cin >> recordedGender;
     std::cin.ignore();
     gender = (recordedGender == 2); 
 
     std::cout << "Q3) How many hours a night do you sleep?\n1. Less than 4 Hours\n2. 4 - 5 Hours\n3. 6 - 7 Hours\n4. 7 - 8 Hours\n5. 8+ Hours\n";
     std::cin >> sleepHours;
     std::cin.ignore();
 
     std::cout << "Q4) How often do you exercise?\n1. Never\n2. 1 - 2 times a week\n3. 3 - 5 times a week\n4. 6 - 7 times a week\n";
     std::cin >> exerciseFrequency;
     std::cin.ignore();
 
     std::cout << "Q5) What diseases run in your family? (1 for Yes, 0 for No)\n"; 
     std::cout << "a) Heart attack or coronary artery disease: ";
     int recordedCAD;
     std::cin >> recordedCAD;
    
     familyDiseasesHistory[0] = (recordedCAD == 1);
     std::cin.ignore();
 
     std::cout << "b) Diabetes (Type 2): ";
     int recordedDiabetes;
     std::cin >> recordedDiabetes;
     familyDiseasesHistory[1] = (recordedDiabetes == 1);
     std::cin.ignore();
 
     std::cout << "c) High cholesterol: ";
     int recordedCholesterol;
     std::cin >> recordedCholesterol;
     familyDiseasesHistory[2] = (recordedCholesterol == 1);
     std::cin.ignore();
 
     std::cout << "d) High blood pressure: ";
     int recordedBP;
     std::cin >> recordedBP;
     familyDiseasesHistory[3] = (recordedBP == 1);
     std::cin.ignore();
 
     std::cout << "Q6) What does your average diet look like?\n";
     std::cout << "1. High Protein Diet (lean meats, fish, eggs, etc.)\n";
     std::cout << "2. Low Carb Diet (vegetables, healthy fats, minimizing breads, etc.)\n";
     std::cout << "3. Vegetarian Diet\n";
     std::cout << "4. Western Diet (highly processed, refined sugars)\n";
     std::cout << "5. Vegan Diet\n";
     std::cout << "6. Balanced Diet\n";
     std::cin >> dietType;
     std::cin.ignore();
 
     std::cout << "Q7) Are you a smoker? (1 for Yes, 0 for No): ";
     int recordedSmoking;
     std::cin >> recordedSmoking;
     isSmoker = (recordedSmoking == 1);
 }
 
 /**
  * @brief Retrieves the age group of the user.
  *
  * @return int The age group as provided by the user.
  */
 int FamilyHealth::getAgeGroup() const { return ageGroup; }
 
 /**
  * @brief Retrieves the gender of the user.
  *
  * @return bool Returns false for female and true for male.
  */
 bool FamilyHealth::getGender() const { return gender; }
 
 /**
  * @brief Retrieves the sleep hours category selected by the user.
  *
  * @return int The sleep hours category.
  */
 int FamilyHealth::getSleepHours() const { return sleepHours; }
 
 /**
  * @brief Retrieves the exercise frequency category of the user.
  *
  * @return int The exercise frequency category.
  */
 int FamilyHealth::getExerciseFrequency() const { return exerciseFrequency; }
 
 /**
  * @brief Retrieves the diet type category of the user.
  *
  * @return int The diet type category.
  */
 int FamilyHealth::getDietType() const { return dietType; }
 
 /**
  * @brief Determines if the user is a smoker.
  *
  * @return bool Returns true if the user is a smoker, false otherwise.
  */
 bool FamilyHealth::getIsSmoker() const { return isSmoker; }
 
 /**
  * @brief Retrieves the count of entries in the family disease history.
  *
  * @return int The number of family disease history entries.
  */
 int FamilyHealth::getFamilyHistoryCount() const {
     return static_cast<int>(familyDiseasesHistory.size());
 }
 
 /**
  * @brief Retrieves a constant reference to the vector of family disease descriptions.
  *
  * @return const std::vector<std::string>& The vector containing descriptions of family diseases.
  */
 const std::vector<std::string>& FamilyHealth::getFamilyDiseases() const {
     return familyDiseases;
 }
 
 /**
  * @brief Checks if a specific disease is present in the family's health history.
  *
  * This function verifies if the disease corresponding to the given index is marked as present (true)
  * in the familyDiseasesHistory vector.
  *
  * @param[in] diseaseIndex The index of the disease to check.
  * @return bool Returns true if the disease is present in the family history, false otherwise.
  *
  * @note If the provided disease index is out of range, the function returns false.
  */
 bool FamilyHealth::hasFamilyDisease(int diseaseIndex) const {
     if (diseaseIndex < 0 || diseaseIndex >= getFamilyHistoryCount()) {
         return false;
     }
     return familyDiseasesHistory[diseaseIndex];
 }
 