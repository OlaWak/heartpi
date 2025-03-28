/**
 * @file NotifyCaregiverScreen.cpp
 * @brief Implements the NotifyCaregiverScreen widget for sending alert emails to caregivers.
 *
 * This file defines the NotifyCaregiverScreen class methods which allow a user to select their account,
 * enter a password, and provide a recipient email to send a HeartPi alert. The screen loads available
 * accounts from a CSV file, verifies credentials, fetches heart rate data, computes risk levels, and composes
 * an alert email using the EmailSender module.
 *
 * @note The CSV file "userdata.csv" is used both for registration (2-column rows) and heart rate data (3-column rows).
 *
 * @author Ola Waked
 */



#include "NotifyCaregiverScreen.h"
#include "EmailSender.h"
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QSet>
#include <QDebug>
#include <QDateTime>



/**
 * @brief Constructs a new NotifyCaregiverScreen object.
 *
 * Sets up the UI elements including title label, account drop-down, password and recipient email fields,
 * and the send/back buttons. Connects button clicks to the corresponding slots.
 *
 * @param stackedWidget Pointer to the QStackedWidget used for screen navigation.
 * @param parent Pointer to the parent QWidget.
 */
NotifyCaregiverScreen::NotifyCaregiverScreen(QStackedWidget *stackedWidget, QWidget *parent)
    : CustomBackgroundWidget(parent),
      stackedWidget(stackedWidget)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    // Title label
    QLabel *title = new QLabel("Send HeartPi Alert", this);
    title->setStyleSheet("color: white; font-size: 28px; font-weight: bold; font-family: 'Poppins', sans-serif;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    // Drop-down for username (from registration rows in userdata.csv)
    accountComboBox = new QComboBox(this);
    accountComboBox->setFixedSize(400, 40);
    accountComboBox->setStyleSheet("padding: 8px; font-size: 16px; border-radius: 10px; background-color: white; color: #333;");
    layout->addWidget(accountComboBox);
    loadAccounts();

    // Password field
    passwordField = new QLineEdit(this);
    passwordField->setPlaceholderText("Enter your password");
    passwordField->setEchoMode(QLineEdit::Password);
    passwordField->setFixedSize(400, 40);
    passwordField->setStyleSheet("padding: 10px; font-size: 16px; border-radius: 15px; border: 2px solid white; background-color: rgba(255,255,255,0.9); color: #333; font-family: 'Poppins', sans-serif;");
    layout->addWidget(passwordField);

    // Recipient Email field
    recipientEmailField = new QLineEdit(this);
    recipientEmailField->setPlaceholderText("Enter recipient email (e.g., caregiver@gmail.com)");
    recipientEmailField->setFixedSize(400, 40);
    recipientEmailField->setStyleSheet("padding: 10px; font-size: 16px; border-radius: 15px; border: 2px solid white; background-color: rgba(255,255,255,0.9); color: #333; font-family: 'Poppins', sans-serif;");
    layout->addWidget(recipientEmailField);

    // Send Alert button
    sendAlertButton = new QPushButton("Send Alert", this);
    sendAlertButton->setFixedSize(300, 60);
    sendAlertButton->setStyleSheet("background-color: white; color: #551900; border: 2px solid #551900; border-radius: 20px; font-size: 16px; font-weight: bold; padding: 8px 16px;");
    layout->addWidget(sendAlertButton, 0, Qt::AlignCenter);

    // Back button
    backButton = new QPushButton("Back", this);
    backButton->setFixedSize(300, 60);
    backButton->setStyleSheet("background-color: white; color: #551900; border: 2px solid #551900; border-radius: 20px; font-size: 16px; font-weight: bold; padding: 8px 16px;");
    layout->addWidget(backButton, 0, Qt::AlignCenter);

    setLayout(layout);

    connect(sendAlertButton, &QPushButton::clicked, this, &NotifyCaregiverScreen::attemptSendAlert);
    connect(backButton, &QPushButton::clicked, this, &NotifyCaregiverScreen::goBack);
}


/**
 * @brief Loads available user accounts from the CSV file.
 *
 * Opens "userdata.csv", reads registration rows (which have exactly 2 columns: username and password),
 * and populates the account drop-down with unique usernames.
 */
void NotifyCaregiverScreen::loadAccounts()
{
    accountComboBox->clear();
    // Open "userdata.csv" to load registration rows (with exactly 2 columns: username,password)
    QFile file("userdata.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "File Error", "Failed to open userdata.csv for loading user names.");
        return;
    }
    QTextStream in(&file);
    bool firstLine = true;
    QSet<QString> accounts;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        if (firstLine) { firstLine = false; continue; }
        QStringList parts = line.split(",");
        if (parts.size() == 2) {
            accounts.insert(parts[0].trimmed());
        }
    }
    file.close();
    for (const QString &account : accounts)
        accountComboBox->addItem(account);
}

/**
 * @brief Verifies the provided credentials against the CSV file.
 *
 * Reads "userdata.csv" and checks registration rows for a match with the provided username and password.
 *
 * @param username The username to verify.
 * @param password The password to verify.
 * @return true if the credentials are valid; false otherwise.
 */
bool NotifyCaregiverScreen::verifyCredentials(const QString &username, const QString &password)
{
    QFile file("userdata.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "File Error", "Could not open userdata.csv for verification!");
        return false;
    }
    QTextStream in(&file);
    bool firstLine = true;
    bool valid = false;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (firstLine) { firstLine = false; continue; }
        QStringList parts = line.split(",");
        // Only consider rows with exactly 2 columns (registration rows)
        if (parts.size() == 2) {
            QString fileUsername = parts[0].trimmed();
            QString filePassword = parts[1].trimmed();
            if (fileUsername.compare(username, Qt::CaseInsensitive) == 0 && filePassword == password) {
                valid = true;
                break;
            }
        }
    }
    file.close();
    return valid;
}

/**
 * @brief Attempts to send an alert email to the caregiver.
 *
 * Validates user input, verifies credentials, and then fetches heart rate data from "userdata.csv".
 * Computes average and latest heart rate, determines the risk level, composes the email subject and body,
 * and sends the email using the EmailSender class. Displays appropriate messages based on success or failure.
 */

void NotifyCaregiverScreen::attemptSendAlert()
{
    QString selectedUser = accountComboBox->currentText();
    QString enteredPassword = passwordField->text().trimmed();
    QString recipientEmail = recipientEmailField->text().trimmed();

    if (selectedUser.isEmpty() || enteredPassword.isEmpty() || recipientEmail.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please select your account, enter your password, and provide a recipient email!");
        return;
    }

    if (!verifyCredentials(selectedUser, enteredPassword)) {
        QMessageBox::warning(this, "Authentication Failed", "Invalid username or password!");
        return;
    }

    // --- Fetch heart rate data from userdata.csv (data rows with 3 columns) ---
    QVector<double> heartRates;
    qint64 latestTimestamp = 0;
    QFile file("userdata.csv");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        bool firstLine = true;
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (firstLine) { firstLine = false; continue; }
            QStringList parts = line.split(",");
            // Data rows have 3 columns: username, timestamp, BPM
            if (parts.size() == 3 && parts[0].trimmed().compare(selectedUser, Qt::CaseInsensitive) == 0) {
                bool ok;
                double bpm = parts[2].toDouble(&ok);
                if (ok)
                    heartRates.append(bpm);
                latestTimestamp = parts[1].toLongLong();
            }
        }
        file.close();
    }

    double avg = 0, latest = 0;
    QString risk = "Unknown";
    if (!heartRates.isEmpty()) {
        double sum = 0;
        for (double bpm : heartRates)
            sum += bpm;
        avg = sum / heartRates.size();
        latest = heartRates.last();

        if (avg < 80)
            risk = "Low";
        else if (avg < 100)
            risk = "Moderate";
        else
            risk = "High";
    }

    // Compose subject and body message.
    QString subject;
    if (risk == "High")
        subject = "\n🚨 HIGH RISK DETECTED, PLEASE CHECK UP ON " +selectedUser+ "'s HEART HEALTH IMMEDIATELY! 🚨\n";
    else
        subject = "HeartPi Alert";

    QString body;
    body += "😊 Hi there!\n\n";
    body += selectedUser + " trusted you with their HeartPi data. Here are their recent readings:\n\n";
    if (!heartRates.isEmpty()) {
        body += "Average Heart Rate: " + QString::number(avg, 'f', 1) + " BPM\n";
        body += "Latest Heart Rate: " + QString::number(latest, 'f', 1) + " BPM\n";
        QDateTime dt = QDateTime::fromSecsSinceEpoch(latestTimestamp);
        body += "Last Reading: " + dt.toString("yyyy-MM-dd hh:mm:ss") + "\n";
        body += "Risk Level: " + risk + "\n\n";
    } else {
        body += "No heart rate data available.\n\n";
    }
    body += "User " + selectedUser + " wanted to share this data with you because they trust you 💖.\n";
   
    body += "\nSent with ❤️ from HeartPi.";

    bool success = EmailSender::sendEmail(recipientEmail, subject, body);
    if (success) {
        QMessageBox::information(this, "Alert Sent", "Alert successfully sent to " + recipientEmail + "!");
    } else {
        QMessageBox::warning(this, "Failed", "Failed to send the alert email.");
    }
}

/**
 * @brief Navigates back to the previous screen.
 *
 * Sets the current index of the stacked widget to 0 (assumed to be the main menu).
 */

void NotifyCaregiverScreen::goBack()
{
    // Assuming the main menu is at index 0.
    if (stackedWidget)
        stackedWidget->setCurrentIndex(0);
}
