/**
 * @file ResultsLoginScreen.cpp
 * @brief Implements the ResultsLoginScreen widget for user login to view previous results.
 *
 * This file defines the ResultsLoginScreen class methods. The widget provides a user interface for users
 * to log in and view their previous results. It loads available account names from a CSV file, validates
 * the entered password based on certain rules, and emits a signal upon successful login.
 *
 * @note The CSV file "userdata.csv" is used for both registration data (two columns: username and password)
 * and heart rate data.
 *
 * @author Ola Waked
 */

#include "ResultsLoginScreen.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QSet>

/**
 * @brief Constructs a new ResultsLoginScreen object.
 *
 * Initializes the ResultsLoginScreen widget with a reference to the stacked widget for navigation and
 * sets up the user interface.
 *
 * @param stackedWidgetRef Pointer to the QStackedWidget used for screen navigation.
 * @param parent Pointer to the parent widget (default is nullptr).
 */

ResultsLoginScreen::ResultsLoginScreen(QStackedWidget *stackedWidgetRef, QWidget *parent)
    : CustomBackgroundWidget(parent), stackedWidget(stackedWidgetRef)
{
    setupUI();
}

/**
 * @brief Sets up the user interface for the login screen.
 *
 * Creates and configures UI elements including a title label, account combo box, password field,
 * login button, and back button. Also arranges these elements using a QVBoxLayout.
 */
void ResultsLoginScreen::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *title = new QLabel("Login to See My Results", this);
    title->setStyleSheet("color: white; font-size: 28px; font-weight: bold; font-family: 'Poppins', sans-serif;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);
    layout->addSpacing(20);

    accountComboBox = new QComboBox(this);
    accountComboBox->setFixedSize(400, 40);
    accountComboBox->setStyleSheet("padding: 8px; font-size: 16px; border-radius: 10px; background-color: white; color: #333;");
    layout->addWidget(accountComboBox);
    layout->addSpacing(10);

    passwordField = new QLineEdit(this);
    passwordField->setPlaceholderText("Enter your password");
    passwordField->setEchoMode(QLineEdit::Password);
    passwordField->setFixedSize(400, 40);
    passwordField->setStyleSheet("padding: 10px; font-size: 16px; border-radius: 15px; border: 2px solid white; background-color: rgba(255,255,255,0.9); color: #333; font-family: 'Poppins', sans-serif;");
    layout->addWidget(passwordField);
    layout->addSpacing(20);

    loginButton = new QPushButton("Login", this);
    loginButton->setFixedSize(300, 60);
    loginButton->setStyleSheet("background-color: white; color: #551900; border: 2px solid #551900; border-radius: 20px; font-size: 16px; font-weight: bold; padding: 8px 16px;");
    layout->addWidget(loginButton, 0, Qt::AlignCenter);
    layout->addSpacing(10);

    backButton = new QPushButton("Back", this);
    backButton->setFixedSize(300, 60);
    backButton->setStyleSheet("background-color: white; color: #551900; border: 2px solid #551900; border-radius: 20px; font-size: 16px; font-weight: bold; padding: 8px 16px;");
    layout->addWidget(backButton, 0, Qt::AlignCenter);

    setLayout(layout);

    connect(loginButton, &QPushButton::clicked, this, &ResultsLoginScreen::attemptLogin);
    connect(backButton, &QPushButton::clicked, this, &ResultsLoginScreen::goBack);
}

/**
 * @brief Updates the account combo box with usernames from the CSV file.
 *
 * Opens "userdata.csv" and reads registration rows (those with exactly 2 columns: username and password).
 * The accountComboBox is then populated with the unique usernames found.
 */
void ResultsLoginScreen::updateAccounts()
{
    accountComboBox->clear();
    // Open the CSV file and read usernames from rows with two columns (registration data)
    QFile file("userdata.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);
    bool firstLine = true;
    QSet<QString> accounts;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty())
            continue;
        if (firstLine) {
            firstLine = false;
            continue;
        }
        QStringList parts = line.split(",");
        if (parts.size() == 2) { // registration row
            accounts.insert(parts[0]);
        }
    }
    file.close();
    // Populate combo box with account names
    for (const QString &account : accounts) {
        accountComboBox->addItem(account);
    }
}

/**
 * @brief Attempts to log in the user based on entered credentials.
 *
 * Validates that both the account and password fields are non-empty, checks password rules
 * (length, containing at least one letter and one digit), and verifies the credentials against the CSV file.
 * If the login is successful, emits the loginSuccessful signal; otherwise, shows an appropriate warning.
 */
void ResultsLoginScreen::attemptLogin()
{
    QString selectedAccount = accountComboBox->currentText();
    QString enteredPassword = passwordField->text().trimmed();
    
    // Check that both fields are non-empty.
    if (selectedAccount.isEmpty() || enteredPassword.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please select an account and enter the password!");
        // Clear fields on a failed attempt.
        passwordField->clear();
        accountComboBox->setCurrentIndex(0);
        return;
    }
    
    // Rule: Password must be more than 4 characters.
    if (enteredPassword.length() <= 4) {
        QMessageBox::warning(this, "Invalid Password", "Password must be more than 4 characters long!");
        passwordField->clear();
        accountComboBox->setCurrentIndex(0);
        return;
    }
    
    // Rule: Password must contain at least one letter and at least one number.
    bool hasLetter = false;
    bool hasDigit = false;
    for (const QChar &ch : enteredPassword) {
        if (ch.isLetter())
            hasLetter = true;
        if (ch.isDigit())
            hasDigit = true;
    }
    if (!hasLetter || !hasDigit) {
        QMessageBox::warning(this, "Invalid Password", "Password must contain at least one letter and one number!");
        passwordField->clear();
        accountComboBox->setCurrentIndex(0);
        return;
    }
    
    // Verify password from CSV file.
    QFile file("userdata.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "File Error", "Could not open userdata.csv!");
        passwordField->clear();
        accountComboBox->setCurrentIndex(0);
        return;
    }
    QTextStream in(&file);
    bool firstLine = true;
    bool valid = false;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (firstLine) { firstLine = false; continue; }
        QStringList parts = line.split(",");
        if (parts.size() == 2) {
            QString username = parts[0];
            QString password = parts[1];
            if (username.compare(selectedAccount, Qt::CaseInsensitive) == 0 &&
                password == enteredPassword) {
                valid = true;
                break;
            }
        }
    }
    file.close();

    if (valid) {
        emit loginSuccessful(selectedAccount);
    } else {
        QMessageBox::warning(this, "Login Failed", "Incorrect password for the selected account!");
    }
    
    // After every attempt, clear the password field and reset the drop-down.
    passwordField->clear();
    accountComboBox->setCurrentIndex(0);
}

/**
 * @brief Navigates back to the main menu.
 *
 * Sets the current index of the stacked widget to 0 (assumed to be the main menu screen).
 */
void ResultsLoginScreen::goBack()
{
    // Assuming the main menu is at index 0 of the stacked widget.
    if (stackedWidget)
        stackedWidget->setCurrentIndex(0);
}
