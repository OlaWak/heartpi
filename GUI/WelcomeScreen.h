#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

/**
 * @file WelcomeScreen.h
 * @brief Declaration of the WelcomeScreen widget.
 *
 * This file declares the WelcomeScreen class, which displays a personalized welcome message
 * along with the user's heart health statistics and a historical chart. It integrates with the
 * HeartHealthScreen to present detailed heart health information.
 *
 * @note This widget is intended to be used within a QStackedWidget for smooth screen transitions.
 *
 * @author Ola Waked
 */

#include "custombackgroundwidget.h"
#include "HeartHealthScreen.h"
#include <QStackedWidget>
#include <QString>
#include <QLabel>


/**
 * @class WelcomeScreen
 * @brief A widget that displays a personalized welcome message and heart health information.
 *
 * The WelcomeScreen class displays a welcome message to the user, retrieves and shows heart rate
 * statistics from a CSV file, and embeds a HeartHealthScreen to display detailed information and charts.
 */
class WelcomeScreen : public CustomBackgroundWidget {
    Q_OBJECT
public:

     /**
     * @brief Constructs a new WelcomeScreen object.
     *
     * Initializes the WelcomeScreen widget with a reference to the QStackedWidget for screen navigation,
     * the current username, and an optional parent widget.
     *
     * @param stackedWidgetRef Pointer to the QStackedWidget managing screen transitions.
     * @param username The current user's username.
     * @param parent Pointer to the parent widget (default is nullptr).
     */
    explicit WelcomeScreen(QStackedWidget *stackedWidgetRef,
                           const QString &username,
                           QWidget *parent = nullptr);

private:
    QStackedWidget *stackedWidget;
    QString user;
    QLabel *welcomeLabel;
    // Re-use the existing HeartHealthScreen to show the graph and info.
    HeartHealthScreen *heartHealthScreen;
};

#endif // WELCOMESCREEN_H
