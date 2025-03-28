/**
 * @file FamilyHealth.h
 * @brief Declaration of the FamilyHealth class for collecting personal and family health data.
 *
 * This file defines the FamilyHealth class, which stores a user's personal health details such as
 * age group, gender, sleep hours, exercise frequency, diet type, and smoking status. It also maintains
 * a record of family disease history using dynamic vectors.
 */

 #ifndef FAMILYHEALTH_H
 #define FAMILYHEALTH_H
 
 #include <vector>
 #include <string>
 
 /**
  * @brief Holds personal and family health data.
  *
  * The FamilyHealth class collects and stores information regarding the user's health and family medical history.
  * It provides methods for gathering input interactively and for retrieving various health metrics.
  */
 class FamilyHealth {
 private:
     int ageGroup;                   /**< Age group selection (e.g., 1: 18-24, 2: 25-34...) */
     bool gender;                    /**< Gender assigned at birth (false: Female, true: Male) */
     int sleepHours;                 /**< Hours of sleep selection (1: less than 4 hours, 2: 4-5 hours, etc.) */
     int exerciseFrequency;          /**< Exercise frequency (1: never, etc.) */
     int dietType;                   /**< Diet type selection */
     bool isSmoker;                  /**< Smoking status (true if smoker, false otherwise) */
     std::vector<std::string> familyDiseases;  /**< Vector storing the disease names */
     std::vector<bool> familyDiseasesHistory;  /**< Vector storing family disease responses (true if present) */
 
 public:
     /**
      * @brief Constructs a new FamilyHealth object.
      *
      * Initializes the familyDiseases vector with predefined disease names and sets the familyDiseasesHistory
      * vector to default values (false).
      */ 
     FamilyHealth();  
     
     /**
      * @brief Gathers health-related information from the user.
      *
      * Prompts the user to enter various health parameters including age, gender, sleep hours,
      * exercise frequency, family disease history, diet type, and smoking status. The responses
      * are stored in the object's private members.
      */
     void gatherInfo();                       
 
     /**
      * @brief Gets the age group of the user.
      *
      * @return int The selected age group.
      */
     int getAgeGroup() const;
 
     /**
      * @brief Gets the gender of the user.
      *
      * @return bool Returns false for Female and true for Male.
      */
     bool getGender() const;
 
     /**
      * @brief Gets the sleep hours selection.
      *
      * @return int The selected sleep hours category.
      */
     int getSleepHours() const;
 
     /**
      * @brief Gets the exercise frequency.
      *
      * @return int The selected exercise frequency category.
      */
     int getExerciseFrequency() const;
 
     /**
      * @brief Gets the diet type.
      *
      * @return int The selected diet type category.
      */
     int getDietType() const;
 
     /**
      * @brief Checks if the user is a smoker.
      *
      * @return bool Returns true if the user is a smoker, false otherwise.
      */
     bool getIsSmoker() const;
     
     /**
      * @brief Checks if a specific disease is present in the family's health history.
      *
      * @param[in] diseaseIndex The index of the disease to check.
      * @return bool Returns true if the disease is present in the family history, false otherwise.
      */
     bool hasFamilyDisease(int diseaseIndex) const;
     
     /**
      * @brief Gets the number of entries in the family disease history.
      *
      * @return int The count of family disease history entries.
      */
     int getFamilyHistoryCount() const; 
     
     /**
      * @brief Gets a constant reference to the vector of family disease names.
      *
      * @return const std::vector<std::string>& A reference to the vector containing the disease names.
      */
     const std::vector<std::string>& getFamilyDiseases() const;
 
     /**
      * @brief Sets the user's age group.
      * @param value The age group value to set.
      */
     void setAgeGroup(int value) { ageGroup = value; }
 
     /**
      * @brief Sets the user's gender.
      * @param value Boolean gender value (false: Female, true: Male).
      */
     void setGender(bool value) { gender = value; }
 
     /**
      * @brief Sets the user's sleep hours category.
      * @param value The sleep hours category value.
      */
     void setSleepHours(int value) { sleepHours = value; }
 
     /**
      * @brief Sets the user's exercise frequency.
      * @param value The exercise frequency category value.
      */
     void setExerciseFrequency(int value) { exerciseFrequency = value; }
 
     /**
      * @brief Sets the user's diet type.
      * @param value The diet type category value.
      */
     void setDietType(int value) { dietType = value; }
 
     /**
      * @brief Sets the user's smoking status.
      * @param value True if the user is a smoker, false otherwise.
      */
     void setIsSmoker(bool value) { isSmoker = value; }
 
     /**
      * @brief Sets a specific family disease history entry.
      * 
      * @param index The index of the disease to update.
      * @param value True if the disease is present in family history, false otherwise.
      * 
      * @note If the index is out of range, the function will not modify anything.
      */
     void setFamilyDiseaseHistory(int index, bool value) {
         if (index >= 0 && index < static_cast<int>(familyDiseasesHistory.size())) {
             familyDiseasesHistory[index] = value;
         }
     }
 };
 
 #endif
 