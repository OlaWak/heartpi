#ifndef NOTIFYCAREGIVERSCREEN_H
#define NOTIFYCAREGIVERSCREEN_H

/**
 * @file NotifyCaregiverScreen.h
 * @brief Declaration of the NotifyCaregiverScreen widget.
 *
 * This header file declares the NotifyCaregiverScreen class, which provides a user interface
 * for sending alert emails to caregivers. It allows the user to select their account, enter a password,
 * and specify a recipient email address. The widget loads account data from a CSV file and verifies
 * user credentials before sending an alert.
 *
 * @author Ola Waked
 */

#include "custombackgroundwidget.h"

class QComboBox;
class QLineEdit;
class QPushButton;
class QStackedWidget;

/**
 * @class NotifyCaregiverScreen
 * @brief A widget for sending alert emails to caregivers.
 *
 * The NotifyCaregiverScreen class allows a user to send a HeartPi alert email by selecting their account,
 * entering a password, and specifying a recipient email address. It loads available accounts from a CSV file
 * and verifies the provided credentials.
 */

class NotifyCaregiverScreen : public CustomBackgroundWidget
{
    Q_OBJECT
public:
    explicit NotifyCaregiverScreen(QStackedWidget *stackedWidget, QWidget *parent = nullptr);

private slots:
    void attemptSendAlert();
    void goBack();

private:
    QStackedWidget *stackedWidget;
    QComboBox *accountComboBox;      // For selecting the username (registration rows)
    QLineEdit *passwordField;         // For entering the password
    QLineEdit *recipientEmailField;   // For entering the recipient's email address
    QPushButton *sendAlertButton;
    QPushButton *backButton;

    // Verifies the credentials from userdata.csv (only considers rows with exactly 2 columns)
    bool verifyCredentials(const QString &username, const QString &password);
    // Loads the account names from userdata.csv (registration rows)
    void loadAccounts();
};

#endif // NOTIFYCAREGIVERSCREEN_H
