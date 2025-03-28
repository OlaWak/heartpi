/**
 * @file mainwindow.cpp
 * @brief Implements the MainWindow class which serves as the central hub for the HeartPi application.
 *
 * The MainWindow sets up the main menu with an animated background and provides navigation
 * between various screens: SurveyScreen for user registration/login, SurveyFormScreen for survey input,
 * HeartHealthScreen for displaying heart health statistics, ResultsLoginScreen for viewing past results,
 * WelcomeScreen for personalized greetings, and NotifyCaregiverScreen for sending alerts.
 *
 * @note The application uses a QStackedWidget to manage screen transitions.
 *
 * @author Ola Waked
 */


#include "mainwindow.h"
#include "custombackgroundwidget.h"
#include "WelcomeScreen.h"
#include "TipsForUser.h"
#include "EmailSender.h"
#include "NotifyCaregiverScreen.h"  // New include
#include <QVBoxLayout>
#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QFontDatabase>
#include <QStackedWidget>
#include <QPushButton>
#include <QFrame>
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QMap>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      stackedWidget(new QStackedWidget(this)),
      mainMenuWidget(new CustomBackgroundWidget(this)), // main menu background widget
      surveyScreen(nullptr),
      surveyFormScreen(nullptr),
      heartHealthScreen(nullptr),
      resultsLoginScreen(nullptr)
{
    setCentralWidget(stackedWidget);

    // 1. Setup main menu widget (with animated heart background)
    setupMainMenu();

    // 2. Create the SurveyScreen (for username/password)
    surveyScreen = new SurveyScreen(stackedWidget, this);

    // 3. Create the HeartHealthScreen (we can pass empty username initially)
    heartHealthScreen = new HeartHealthScreen(stackedWidget, "", this);

    // 4. Create the ResultsLoginScreen for viewing results
    resultsLoginScreen = new ResultsLoginScreen(stackedWidget, this);

    // Add these widgets to the stack
    stackedWidget->addWidget(mainMenuWidget);
    stackedWidget->addWidget(surveyScreen);
    stackedWidget->addWidget(heartHealthScreen);
    stackedWidget->addWidget(resultsLoginScreen);

    connect(surveyScreen, &SurveyScreen::surveyLoginSuccessful,
        this, [this](const QString &username) {
            currentUsername = username;
            if (heartHealthScreen)
                heartHealthScreen->setUser(username);
            showSurveyForm();
        });

    connect(surveyScreen, &SurveyScreen::goBackToMain,
            this, &MainWindow::returnToMain);

    connect(heartHealthScreen, &HeartHealthScreen::backToMain,
            this, &MainWindow::returnToMain);

    // Updated connection: Create a WelcomeScreen when login is successful.
    connect(resultsLoginScreen, &ResultsLoginScreen::loginSuccessful,
            this, [=](const QString &username) {
                WelcomeScreen *welcomeScreen = new WelcomeScreen(stackedWidget, username, this);
                stackedWidget->addWidget(welcomeScreen);
                stackedWidget->setCurrentWidget(welcomeScreen);
    });
}

MainWindow::~MainWindow()
{
    // Qt automatically deletes child widgets.
}

void MainWindow::setupMainMenu()
{
    QVBoxLayout *layout = new QVBoxLayout(mainMenuWidget);
    layout->setAlignment(Qt::AlignCenter);

    QFrame *container = new QFrame(mainMenuWidget);
    container->setStyleSheet("background-color: #662255; border-radius: 20px;");
    container->setFixedSize(500, 500);

    QVBoxLayout *containerLayout = new QVBoxLayout(container);
    containerLayout->setAlignment(Qt::AlignCenter);

    QLabel *logoLabel = new QLabel(container);
    QPixmap logoPixmap("images/heartpilogo.png");
    logoPixmap = logoPixmap.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logoLabel->setPixmap(logoPixmap);
    logoLabel->setAlignment(Qt::AlignCenter);

    QPushButton *startButton = new QPushButton("Start Assessment", container);
    QPushButton *loginButton  = new QPushButton("Login to See My Results", container);

    QString buttonStyle = "QPushButton {"
                          "  background-color: white;"
                          "  color: #551900;"
                          "  border: 2px solid #551900;"
                          "  border-radius: 20px;"
                          "  font-size: 16px;"
                          "  font-weight: bold;"
                          "  padding: 8px 16px;"
                          "  min-width: 250px;"
                          "  font-family: 'Poppins', sans-serif;"
                          "}";
    startButton->setStyleSheet(buttonStyle);
    loginButton->setStyleSheet(buttonStyle);
    startButton->setFixedSize(300, 60);
    loginButton->setFixedSize(300, 60);

    containerLayout->addWidget(logoLabel);
    containerLayout->addSpacing(20);
    containerLayout->addWidget(startButton, 0, Qt::AlignCenter);
    containerLayout->addWidget(loginButton,  0, Qt::AlignCenter);

    // --- Notify a Caregiver Button ---
    QPushButton *notifyButton = new QPushButton("Notify a Caregiver", container);
    notifyButton->setStyleSheet(buttonStyle);
    notifyButton->setFixedSize(300, 60);
    containerLayout->addWidget(notifyButton, 0, Qt::AlignCenter);

    layout->addWidget(container, 0, Qt::AlignCenter);

    // Start the animated background once (and let it run continuously)
    mainMenuWidget->startAnimation();

    connect(startButton, &QPushButton::clicked, this, &MainWindow::showSurveyScreen);
    connect(loginButton,  &QPushButton::clicked, this, &MainWindow::showResultsLoginScreen);

    // --- Notify Button Connection: switch to NotifyCaregiverScreen ---
    // --- Notify a Caregiver Button Connection ---
    connect(notifyButton, &QPushButton::clicked, [=]() {
        // Create and switch to the NotifyCaregiverScreen.
        NotifyCaregiverScreen *notifyScreen = new NotifyCaregiverScreen(stackedWidget);
        stackedWidget->addWidget(notifyScreen);
        stackedWidget->setCurrentWidget(notifyScreen);
    });


    // ----------- Exit Button at Bottom Right Without Breaking Layout ------------
    QPushButton *exitButton = new QPushButton("Exit", mainMenuWidget);
    exitButton->setStyleSheet(buttonStyle);
    QString exitButtonStyle = "QPushButton {"
                              "  background-color: white;"
                              "  color: #551900;"
                              "  border: 2px solid #551900;"
                              "  border-radius: 15px;"
                              "  font-size: 14px;"
                              "  font-weight: bold;"
                              "  padding: 4px 8px;"
                              "  font-family: 'Poppins', sans-serif;"
                              "}";
    exitButton->setStyleSheet(exitButtonStyle);
    exitButton->setFixedSize(150, 60);
    exitButton->raise();
    connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);

    // Initial position (bottom-right)
    exitButton->move(mainMenuWidget->width() - exitButton->width() - 20,
                     mainMenuWidget->height() - exitButton->height() - 20);

    // Keep it bottom-right on resize.
    connect(mainMenuWidget, &CustomBackgroundWidget::resized, [=]() {
        exitButton->move(mainMenuWidget->width() - exitButton->width() - 20,
                         mainMenuWidget->height() - exitButton->height() - 20);
    });
    // ----------------------------------------------------------------------------

    // --- Project Credits & Warning (bottom of screen) ---
    QLabel *footerLabel = new QLabel(mainMenuWidget);
    footerLabel->setAlignment(Qt::AlignCenter);
    footerLabel->setStyleSheet("color: white; font-size: 13px; font-family: 'Poppins', sans-serif;");
    footerLabel->setText(
        "CS 3307 - Team 40<br>"
        "<span style='font-size:12px;'>Team Members – Yosra Alim, Sena Debian, Sarah Solaiman, Ola Waked, and Shivali Sharma</span><br><br>"
        "⚠️ <b>WARNING:</b> This HeartPi project is for the <b>CS3307</b> course at Western University only.<br>"
        "It is <b>not</b> a professional medical application. Please consult a healthcare provider for medical advice."
    );
    
    layout->addSpacing(20);
    layout->addWidget(footerLabel, 0, Qt::AlignBottom | Qt::AlignHCenter);
}

void MainWindow::showSurveyScreen()
{
    stackedWidget->setCurrentWidget(surveyScreen);
}

void MainWindow::showSurveyForm()
{
    if (!surveyFormScreen) {
        surveyFormScreen = new SurveyFormScreen(stackedWidget, currentUsername, this);
        connect(surveyFormScreen, &SurveyFormScreen::surveyCompleted,
                heartHealthScreen, &HeartHealthScreen::displayResults);
        stackedWidget->addWidget(surveyFormScreen);
    }
    stackedWidget->setCurrentWidget(surveyFormScreen);
}

void MainWindow::returnToMain()
{
    if (surveyScreen) {
        surveyScreen->reset();
    }
    if (surveyFormScreen) {
        surveyFormScreen->reset();
    }
    if (heartHealthScreen) {
        heartHealthScreen->reset();
    }
    currentUsername.clear();

    stackedWidget->setCurrentIndex(0);
}

void MainWindow::showResultsLoginScreen()
{
    resultsLoginScreen->updateAccounts();
    stackedWidget->setCurrentWidget(resultsLoginScreen);
}
