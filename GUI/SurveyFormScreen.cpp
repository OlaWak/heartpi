#include "SurveyFormScreen.h"
#include "HeartHealthScreen.h"
#include <QMessageBox>

/**
 * @file SurveyFormScreen.cpp
 * @brief Implements the SurveyFormScreen widget for collecting heart health survey data.
 *
 * This file defines the SurveyFormScreen class methods. The widget displays a survey form with various
 * questions regarding the user's health, collects the responses, and then emits a signal with the
 * collected FamilyHealth data. Upon submission, it transitions to the HeartHealthScreen to display results.
 *
 * @note The survey questions include age group, gender, sleep hours, exercise frequency, family history,
 * diet type, and smoking status.
 *
 * @author Ola Waked
 */

 /**
 * @brief Constructs a new SurveyFormScreen object.
 *
 * Initializes the SurveyFormScreen widget with a reference to the QStackedWidget used for navigation,
 * sets the current username, and calls setupUI() to initialize the user interface.
 *
 * @param stackedWidgetRef Pointer to the QStackedWidget used for screen transitions.
 * @param username The username of the current user.
 * @param parent Pointer to the parent widget (default is nullptr).
 */
SurveyFormScreen::SurveyFormScreen(QStackedWidget *stackedWidgetRef, const QString &username, QWidget *parent)
    : CustomBackgroundWidget(parent), stackedWidget(stackedWidgetRef), user(username)
{
    setupUI();
}

void SurveyFormScreen::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *title = new QLabel("Heart Health Assessment", this);
    title->setStyleSheet("color: white; font-size: 28px; font-weight: bold; font-family: 'Poppins', sans-serif;");
    title->setAlignment(Qt::AlignCenter);

    layout->addWidget(title);
    layout->addSpacing(20);

    // Survey Questions and Answer Options
    QStringList questions = {
        "What is your age group?",
        "What is your gender at birth?",
        "How many hours do you sleep per night?",
        "How often do you exercise?",
        "Do you have a family history of heart disease?",
        "What is your diet type?",
        "Are you a smoker?"
    };

    QVector<QStringList> options = {
        {"18 - 24", "25 - 34", "35 - 44", "45 - 54", "55 - 64", "65+"},
        {"Female", "Male"},
        {"Less than 4", "4 - 5", "6 - 7", "7 - 8", "More than 8"},
        {"Never", "1 - 2 times a week", "3 - 5 times a week", "6 - 7 times a week"},
        {"No", "Yes"},
        {"High Protein", "Low Carb", "Vegetarian", "Western Diet", "Vegan", "Balanced Diet"},
        {"No", "Yes"}
    };

    for (int i = 0; i < questions.size(); ++i) {
        QLabel *label = new QLabel(questions[i], this);
        label->setStyleSheet("color: white; font-size: 18px; font-family: 'Poppins', sans-serif;");
        label->setAlignment(Qt::AlignLeft);

        QComboBox *comboBox = new QComboBox(this);
        comboBox->addItems(options[i]);
        comboBox->setStyleSheet("padding: 8px; font-size: 16px; border-radius: 10px; background-color: white; color: #333;");
        comboBox->setFixedSize(400, 40);

        questionFields.append(comboBox);
        layout->addWidget(label);
        layout->addWidget(comboBox);
        layout->addSpacing(10);
    }

    // Submit Button
    QPushButton *submitButton = new QPushButton("Submit Survey", this);
    submitButton->setFixedSize(300, 60);
    submitButton->setStyleSheet("background-color: white; color: #551900; border: 2px solid #551900; "
                                "border-radius: 20px; font-size: 16px; font-weight: bold; padding: 8px 16px;");

    layout->addSpacing(20);
    layout->addWidget(submitButton, 0, Qt::AlignCenter);

    connect(submitButton, &QPushButton::clicked, this, &SurveyFormScreen::submitSurvey);

    setLayout(layout);
}
/**
 * @brief Submits the survey and processes the collected data.
 *
 * Retrieves responses from the survey's combo boxes, sets the corresponding values in a FamilyHealth
 * object, and emits the surveyCompleted signal with the collected data. It then finds the HeartHealthScreen
 * in the stacked widget to display the results.
 */
void SurveyFormScreen::submitSurvey()
{
    familyData.setAgeGroup(questionFields[0]->currentIndex() + 1);
    familyData.setGender(questionFields[1]->currentIndex() == 1);
    familyData.setSleepHours(questionFields[2]->currentIndex() + 1);
    familyData.setExerciseFrequency(questionFields[3]->currentIndex() + 1);
    familyData.setDietType(questionFields[5]->currentIndex() + 1);
    familyData.setIsSmoker(questionFields[6]->currentIndex() == 1);
    familyData.setFamilyDiseaseHistory(0, questionFields[4]->currentIndex() == 1);

    emit surveyCompleted(familyData);

    if (stackedWidget) {
        HeartHealthScreen *heartHealthScreen = stackedWidget->findChild<HeartHealthScreen *>();
        if (heartHealthScreen) {
            heartHealthScreen->displayResults(familyData);
            stackedWidget->setCurrentWidget(heartHealthScreen);
        } else {
            qWarning("HeartHealthScreen not found in stackedWidget!");
        }
    }
}

/**
 * @brief Resets the survey form to its default state.
 *
 * Resets each survey question (combo box) to its default selection (index 0), allowing the user
 * to retake the survey if needed.
 */
void SurveyFormScreen::reset()
{
    // Reset each question field to the default (index 0)
    for (QComboBox *comboBox : qAsConst(questionFields)) {
        if (comboBox) {
            comboBox->setCurrentIndex(0);
        }
    }
}
