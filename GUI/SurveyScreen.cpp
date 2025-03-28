#include "Surveyscreen.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>


/**
 * @file SurveyScreen.cpp
 * @brief Implements the SurveyScreen widget for user registration and login.
 *
 * This file defines the SurveyScreen class methods which provide an interface for new users to
 * register or log in by entering a username and password. The widget validates the input, checks for
 * existing usernames in a CSV file ("userdata.csv"), and saves new user data. Upon successful
 * registration, it emits a signal indicating a successful survey login.
 *
 * @note The CSV file stores user registration information, including a header line if the file is new.
 *
 * @author Ola Waked
 */

 /**
 * @brief Constructs a new SurveyScreen object.
 *
 * Initializes the SurveyScreen widget by setting up the UI elements such as labels, text fields, and buttons.
 * The widget is centered using a QVBoxLayout and connects the start and back buttons to their respective slots.
 *
 * @param stackedWidgetRef Pointer to the QStackedWidget used for screen navigation.
 * @param parent Pointer to the parent widget (default is nullptr).
 */
SurveyScreen::SurveyScreen(QStackedWidget *stackedWidgetRef, QWidget *parent)
    : CustomBackgroundWidget(parent), stackedWidget(stackedWidgetRef)
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *welcomeLabel = new QLabel("Welcome!", this);
    welcomeLabel->setStyleSheet("color: white; font-size: 32px; font-weight: 600; font-family: 'Poppins', sans-serif;");
    welcomeLabel->setAlignment(Qt::AlignCenter);

    usernameField = new QLineEdit(this);
    usernameField->setPlaceholderText("Enter your username");
    usernameField->setFixedSize(450, 55);
    usernameField->setStyleSheet("padding: 10px; font-size: 16px; border-radius: 15px; "
                                 "border: 2px solid white; background-color: rgba(255,255,255,0.9); "
                                 "color: #333; font-family: 'Poppins', sans-serif;");

    passwordField = new QLineEdit(this);
    passwordField->setPlaceholderText("Enter your password");
    passwordField->setEchoMode(QLineEdit::Password);
    passwordField->setFixedSize(450, 55);
    passwordField->setStyleSheet("padding: 10px; font-size: 16px; border-radius: 15px; "
                                 "border: 2px solid white; background-color: rgba(255,255,255,0.9); "
                                 "color: #333; font-family: 'Poppins', sans-serif;");

    QString buttonStyle = "QPushButton {"
                          "  background-color: white;"
                          "  color: #551900;"
                          "  border: 2px solid #551900;"
                          "  border-radius: 20px;"
                          "  font-size: 16px;"
                          "  font-weight: bold;"
                          "  padding: 8px 16px;"
                          "  min-width: 160px;"
                          "  font-family: 'Poppins', sans-serif;"
                          "}";

    startSurveyButton = new QPushButton("Start", this);
    startSurveyButton->setFixedSize(300, 60);
    startSurveyButton->setStyleSheet(buttonStyle);

    backButton = new QPushButton("Back", this);
    backButton->setFixedSize(300, 60);
    backButton->setStyleSheet(buttonStyle);

    layout->addWidget(welcomeLabel);
    layout->addSpacing(15);
    layout->addWidget(usernameField);
    layout->addSpacing(10);
    layout->addWidget(passwordField);
    layout->addSpacing(20);
    layout->addWidget(startSurveyButton, 0, Qt::AlignCenter);
    layout->addWidget(backButton, 0, Qt::AlignCenter);

    setLayout(layout);

    connect(startSurveyButton, &QPushButton::clicked, this, &SurveyScreen::saveUserData);
    connect(backButton, &QPushButton::clicked, this, &SurveyScreen::goBackToMain);
}

void SurveyScreen::saveUserData()
{
    QString username = usernameField->text().trimmed();
    QString password = passwordField->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Both fields must be filled!");
        return;
    }

    // Password validation
    QRegularExpression regex("^(?=.*[A-Za-z])(?=.*\\d).{5,}$");
    if (!regex.match(password).hasMatch()) {
        QMessageBox::warning(this, "Password Error", "Password must be at least 5 characters long and contain at least one letter and one number.");
        return;
    }

    QFile file("userdata.csv");
    bool userAlreadyExists = false;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        bool firstLine = true;
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (firstLine) {
                firstLine = false;
                continue;
            }
            QStringList parts = line.split(",");
            if (!parts.isEmpty() && parts[0].compare(username, Qt::CaseInsensitive) == 0) {
                userAlreadyExists = true;
                break;
            }
        }
        file.close();
    }

    if (userAlreadyExists) {
        QMessageBox::warning(this, "Registration Error", "Username already exists! Please choose another.");
        return;
    }

    bool fileExists = file.exists();
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        if (!fileExists) {
            out << "Username,Password\n";
        }
        out << username << "," << password << "\n";
        file.close();

        emit surveyLoginSuccessful(username);
    } else {
        QMessageBox::warning(this, "Error", "Failed to save data to file.");
    }
}

void SurveyScreen::reset()
{
    if (usernameField) {
        usernameField->clear();
    }
    if (passwordField) {
        passwordField->clear();
    }
}
