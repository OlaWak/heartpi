#include "custombackgroundwidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QtMath>

/**
 * @file custombackgroundwidget.cpp
 * @brief Implements the CustomBackgroundWidget class.
 *
 * The CustomBackgroundWidget provides a custom background with an animated heartbeat wave.
 * The widget uses a QTimer to update the animation and overrides the paintEvent to draw a
 * linear gradient background and a heartbeat waveform when the animation is active.
 *
 * @note The animation is not started automatically. Use startAnimation() to begin and stopAnimation() to end the animation.
 *
 * @author Ola Waked
 */

 /**
 * @brief Constructs a new CustomBackgroundWidget.
 *
 * Initializes the widget and sets the initial offset for the animation. Also creates a QTimer
 * that updates the widget periodically to animate the heartbeat wave.
 *
 * @param parent Pointer to the parent widget.
 */
CustomBackgroundWidget::CustomBackgroundWidget(QWidget *parent)
    : QWidget(parent), offsetX(0)
{
    // Timer for the animation
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        offsetX += 10;
        if (offsetX > width()) {
            offsetX = 0;
        }
        update();
    });

    // We Do NOT start animation immediately
}

// **Start the animation**
void CustomBackgroundWidget::startAnimation()
{
    if (!timer->isActive()) {
        timer->start(40); // 25 FPS
    }
}

// **Stop the animation**
void CustomBackgroundWidget::stopAnimation()
{
    if (timer->isActive()) {
        timer->stop();
    }
}

void CustomBackgroundWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Background Gradient
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0.0, QColor("#532e63"));
    gradient.setColorAt(1.0, QColor("#98002e"));
    painter.fillRect(rect(), gradient);

    // **If animation is running, draw the heartbeat wave**
    if (timer->isActive()) {
        QPen pen(QColor("#ffffff"));
        pen.setWidth(5);
        painter.setPen(pen);

        QPainterPath path;
        int midY = height() / 2;
        int amplitude = height() / 4;

        path.moveTo(0, midY);

        for (int x = 0; x < width(); x += 20) {
            double y;
            int pattern = (x + offsetX) % 200;

            if (pattern < 50) { 
                y = midY;
            } else if (pattern < 75) { 
                y = midY - amplitude * 1.7;
            } else if (pattern < 100) { 
                y = midY;
            } else if (pattern < 125) { 
                y = midY + amplitude * 1.5;
            } else {
                y = midY;
            }

            path.lineTo(x, y);
        }

        painter.setOpacity(0.25);
        painter.drawPath(path);
        painter.setOpacity(1.0);
    }
}

void CustomBackgroundWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    emit resized();
}
