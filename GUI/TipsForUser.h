#ifndef TIPSFORUSER_H
#define TIPSFORUSER_H

/**
 * @file TipsForUser.h
 * @brief Declaration of the TipsForUser widget.
 *
 * This file declares the TipsForUser class, which displays curated health tips to help users improve their heart health.
 * The tips are tailored based on the user's risk level and are presented within a custom-styled widget. The widget also
 * supports navigation back to a previous screen.
 *
 * @note The widget overrides the paintEvent() function to render a custom linear gradient background.
 *
 * @author Ola Waked
 */
#include <QWidget>
#include <QStackedWidget>

/**
 * @class TipsForUser
 * @brief A widget for displaying health tips based on risk level.
 *
 * The TipsForUser class provides a user interface to display various health tips, organized into categories,
 * such as nutrition, activity, sleep, and stress management. The tips are adjusted based on the risk level
 * provided to the widget's constructor. It also maintains a pointer to the previous screen to allow easy navigation.
 */

class TipsForUser : public QWidget {
    Q_OBJECT
public:
    // Now accepts a previousScreen pointer.
    explicit TipsForUser(QStackedWidget *stackedWidgetRef, const QString &riskLevel, QWidget *previousScreen, QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QStackedWidget *stackedWidget;
    QWidget *previousScreen; // The screen to return to.
};

#endif // TIPSFORUSER_H
