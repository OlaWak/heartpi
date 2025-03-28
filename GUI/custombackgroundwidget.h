#ifndef CUSTOMBACKGROUNDWIDGET_H
#define CUSTOMBACKGROUNDWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
/**
 * @file CustomBackgroundWidget.h
 * @brief Declaration of the CustomBackgroundWidget class.
 *
 * This file declares the CustomBackgroundWidget class, which provides a custom background
 * for widgets along with an optional heartbeat animation. It is designed to be used as a base
 * class for widgets that require a styled, animated background.
 *
 * @note The widget uses a QTimer to animate the heartbeat wave. The animation can be started
 * and stopped using startAnimation() and stopAnimation(), respectively.
 *
 * @author Ola Waked
 */

 /**
 * @class CustomBackgroundWidget
 * @brief A widget with a custom background and animated heartbeat wave.
 *
 * The CustomBackgroundWidget class inherits from QWidget and provides a custom-painted background
 * featuring a linear gradient. When animation is enabled, it displays a heartbeat wave animation.
 * It emits a resized() signal whenever the widget is resized.
 */
class CustomBackgroundWidget : public QWidget {
    Q_OBJECT
public:
/**
     * @brief Constructs a new CustomBackgroundWidget object.
     *
     * Initializes the widget with an optional parent widget and sets the initial offset for the animation.
     *
     * @param parent Pointer to the parent widget (default is nullptr).
     */
    explicit CustomBackgroundWidget(QWidget *parent = nullptr);

    void startAnimation(); // Start the heartbeat animation
    void stopAnimation();  // Stop the heartbeat animation

signals:
    void resized(); // Signal to reposition exit button when resized

protected:
    void paintEvent(QPaintEvent *event) override;       // Draws the background & heartbeat wave
    void resizeEvent(QResizeEvent *event) override;     // Needed to emit resized() properly

private:
    QTimer *timer;
    int offsetX;
};

#endif // CUSTOMBACKGROUNDWIDGET_H
