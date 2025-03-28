#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/**
 * @mainpage HeartPi Project Documentation
 *
 * HeartPi is a simulation project developed for CS3307 (by Team 40) that models heart health monitoring
 * by combining survey-based user inputs with randomized sensor data.
 *
 * The application features an interactive Qt-based GUI to display live heart rate graphs, risk assessments,
 * and additional information such as data logging and email notifications.
 */

/**
 * @file MainWindow.h
 * @brief Declaration of the MainWindow class.
 *
 * The MainWindow class is the primary window for the HeartPi application. It manages navigation among
 * various screens (survey, survey form, heart health, and results login screens) using a QStackedWidget.
 *
 * @note This project is intended for the CS3307 course at Western University.
 *
 * @author Ola Waked
 */

#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include "SurveyScreen.h"
#include "SurveyFormScreen.h"
#include "HeartHealthScreen.h"
#include "ResultsLoginScreen.h"  // new header

/**
 * @class MainWindow
 * @brief The main window of the HeartPi application.
 *
 * MainWindow sets up the main menu with an animated background and handles navigation between
 * different screens of the application, including user registration/login and heart health assessment.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
/**
     * @brief Constructs a new MainWindow object.
     *
     * Initializes the main window, creates and adds the necessary screens to the QStackedWidget,
     * and sets up signal-slot connections for navigation.
     *
     * @param parent Pointer to the parent widget (default is nullptr).
     */
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showSurveyScreen();
    void showSurveyForm();
    void returnToMain();
    void showResultsLoginScreen();  // new slot

private:
 /**
     * @brief Sets up the main menu interface.
     *
     * Creates the main menu widget with an animated background and adds navigation buttons.
     */
    void setupMainMenu();

    QStackedWidget *stackedWidget;                  ///< Manages the application's screen transitions.
    CustomBackgroundWidget *mainMenuWidget;         ///< Main menu widget with an animated background.
    SurveyScreen      *surveyScreen;                 ///< Screen for user registration/login.
    SurveyFormScreen  *surveyFormScreen;
    HeartHealthScreen *heartHealthScreen;
    ResultsLoginScreen *resultsLoginScreen; // new widget

    QString currentUsername;
};

#endif // MAINWINDOW_H
