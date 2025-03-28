#ifndef SURVEYSCREEN_H
#define SURVEYSCREEN_H

/**
 * @file SurveyScreen.h
 * @brief Declaration of the SurveyScreen widget.
 *
 * This header file declares the SurveyScreen class, which provides an interface for users to register or
 * log in before taking the heart health survey. The widget includes fields for username and password along
 * with buttons to start the survey or navigate back. On successful registration, it emits a signal indicating
 * that the survey login was successful.
 *
 * @note This widget is designed to be used within a QStackedWidget for navigation between screens.
 *
 * @author Ola Waked
 */
#include "custombackgroundwidget.h"
#include "SurveyFormScreen.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QString>
#include <QWidget>

/**
 * @class SurveyScreen
 * @brief A widget for user registration/login for the heart health survey.
 *
 * The SurveyScreen class provides input fields for a username and password, along with buttons to start the survey
 * or go back to the main screen. It validates the inputs, saves user data to a CSV file, and emits a signal upon
 * successful registration.
 */

class SurveyScreen : public CustomBackgroundWidget
{
    Q_OBJECT
public:
    explicit SurveyScreen(QStackedWidget *stackedWidgetRef, QWidget *parent = nullptr);

    // Reset input fields
    void reset();

signals:
    void goBackToMain();
    void surveyLoginSuccessful(const QString &username);

private slots:
 /**
     * @brief Saves the user data entered in the survey screen.
     *
     * Validates the input fields, checks the user data against a CSV file, and saves new user data if valid.
     * Emits the surveyLoginSuccessful signal on success.
     */
    void saveUserData();

private:
    QLineEdit     *usernameField;
    QLineEdit     *passwordField;
    QPushButton   *startSurveyButton;
    QPushButton   *backButton;
    QStackedWidget *stackedWidget;
};

#endif // SURVEYSCREEN_H
