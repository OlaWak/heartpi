#ifndef RESULTSLOGINSCREEN_H
#define RESULTSLOGINSCREEN_H


/**
 * @file ResultsLoginScreen.h
 * @brief Declaration of the ResultsLoginScreen widget.
 *
 * This file declares the ResultsLoginScreen class, which provides a user interface for logging in to view
 * previous results. The widget includes a combo box for account selection, a password field, and buttons
 * for login and navigation. Upon successful login, a signal is emitted with the username.
 *
 * @note The class typically reads account information from a CSV file (e.g., "userdata.csv").
 *
 * @author Ola Waked
 */
#include "custombackgroundwidget.h"
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>

/**
 * @class ResultsLoginScreen
 * @brief A widget for user login to view previous results.
 *
 * The ResultsLoginScreen class provides an interface for users to log in by selecting an account and entering a password.
 * It verifies the credentials and emits a signal upon a successful login.
 */

class ResultsLoginScreen : public CustomBackgroundWidget
{
    Q_OBJECT
public:
/**
     * @brief Constructs a new ResultsLoginScreen object.
     *
     * Initializes the login screen with a reference to the stacked widget for navigation and sets up the UI elements.
     *
     * @param stackedWidgetRef Pointer to the QStackedWidget used for screen navigation.
     * @param parent Pointer to the parent widget (default is nullptr).
     */
    explicit ResultsLoginScreen(QStackedWidget *stackedWidgetRef, QWidget *parent = nullptr);

    /**
     * @brief Updates the account list.
     *
     * Reads available account names from a CSV file and populates the account combo box.
     */
    void updateAccounts();

signals:
    /**
     * @brief Signal emitted upon a successful login.
     *
     * @param username The username of the account that successfully logged in.
     */
    void loginSuccessful(const QString &username);

private slots:

    /**
     * @brief Attempts to log in the user.
     *
     * Validates the entered credentials and emits the loginSuccessful signal if the login is successful.
     */
    void attemptLogin();

    /**
     * @brief Navigates back to the previous screen.
     */
    void goBack();

private:
    /**
     * @brief Sets up the user interface.
     *
     * Creates and arranges UI components such as the account combo box, password field, login button, and back button.
     */
    void setupUI();

    QStackedWidget *stackedWidget;
    QComboBox *accountComboBox;
    QLineEdit *passwordField;
    QPushButton *loginButton;
    QPushButton *backButton;
};

#endif // RESULTSLOGINSCREEN_H
