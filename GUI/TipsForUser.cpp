#include "TipsForUser.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QScrollArea>
#include <QPainter>
#include <QFont>

/**
 * @file TipsForUser.cpp
 * @brief Implements the TipsForUser widget that displays curated health tips for a healthier heart.
 *
 * This file defines the constructor and paintEvent() function for the TipsForUser widget.
 * The widget displays various health tips organized into categories such as activity, nutrition,
 * monitoring, and sleep & stress. Tips are tailored based on the user's risk level ("High", "Moderate",
 * or other), and a scrollable layout is used to present the content. A back button is provided to navigate
 * to the previous screen.
 *
 * @note The widget's background is painted with a custom linear gradient.
 *
 * @author Ola Waked
 */


 /**
 * @brief Constructs a new TipsForUser widget.
 *
 * Creates a TipsForUser widget that displays health tips based on the provided risk level.
 * The constructor sets up the main vertical layout with a title, a scrollable area containing
 * various tip categories and tip boxes, and a back button that navigates to the previous screen.
 *
 * @param stackedWidgetRef Pointer to the QStackedWidget used for screen navigation.
 * @param riskLevel The risk level (e.g., "High", "Moderate", or other) used to tailor the displayed tips.
 * @param prevScreen Pointer to the previous widget screen for navigation.
 * @param parent Pointer to the parent widget (default is nullptr).
 */
TipsForUser::TipsForUser(QStackedWidget *stackedWidgetRef, const QString &riskLevel, QWidget *prevScreen, QWidget *parent)
    : QWidget(parent), stackedWidget(stackedWidgetRef), previousScreen(prevScreen)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(20);

    QLabel *title = new QLabel("Tips for a Healthier Heart", this);
    title->setStyleSheet("font-size: 28px; font-weight: bold; color: white;");
    mainLayout->addWidget(title, 0, Qt::AlignTop | Qt::AlignHCenter);

    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);

    QWidget *scrollContent = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);
    scrollLayout->setSpacing(25);

    auto addCategoryTitle = [&](const QString &emojiTitle) {
        QLabel *cat = new QLabel(emojiTitle);
        QFont font("Segoe UI Symbol", 20, QFont::Bold);
        cat->setFont(font);
        cat->setStyleSheet("color: white;");
        scrollLayout->addWidget(cat);
    };

    auto addTipBox = [&](const QString &title, const QString &description = "", bool urgent = false) {
        QFrame *box = new QFrame();
        box->setStyleSheet(QString("background-color: rgba(255, 255, 255, 0.1); "
                                   "border-radius: 20px; padding: 20px; border: %1;")
                                   .arg(urgent ? "2px solid #ff9999" : "1px solid rgba(255,255,255,0.3)"));

        QVBoxLayout *boxLayout = new QVBoxLayout(box);

        QLabel *labelTitle = new QLabel(title, box);
        labelTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: white;");
        labelTitle->setWordWrap(true);
        QFont emojiFont("Segoe UI Symbol");
        labelTitle->setFont(emojiFont);
        boxLayout->addWidget(labelTitle);

        if (!description.isEmpty()) {
            QLabel *desc = new QLabel(description, box);
            desc->setStyleSheet("font-size: 14px; color: #e0e0e0;");
            desc->setWordWrap(true);
            desc->setFont(emojiFont);
            boxLayout->addWidget(desc);
        }

        scrollLayout->addWidget(box);
    };

    // CURATED TIPS BASED ON RISK
    if (riskLevel == "High") {
        addTipBox("High Risk Detected: Please consult your doctor immediately.",
                  "Your heart may be under strain. Seeking medical advice is a crucial step.", true);

        addCategoryTitle("😅 Activity");
        addTipBox("Engage in at least 30 minutes of physical activity daily.",
                  "This boosts circulation and strengthens your heart muscle.");

        addCategoryTitle("😋 Nutrition");
        addTipBox("Eat more vegetables, lean meats, and low-sodium meals.",
                  "A nutrient-rich diet helps reduce cholesterol and blood pressure.");
        addTipBox("Avoid tobacco and smoking completely.",
                  "Smoking drastically increases the risk of heart attacks and strokes.");

        addCategoryTitle("😷 Monitoring");
        addTipBox("Track blood pressure, weight, and cholesterol regularly.",
                  "Monitoring helps catch issues early and stay on top of your health.");

        addCategoryTitle("😴 Sleep & Stress");
        addTipBox("Get at least 7 hours of quality sleep.",
                  "Sleep helps your body recover and maintain healthy heart rhythms.");
        addTipBox("Manage stress through deep breathing, prayer, or journaling.",
                  "Stress increases heart rate and blood pressure — managing it is key.");
    }
    else if (riskLevel == "Moderate") {
        addCategoryTitle("😅 Activity");
        addTipBox("150 mins/week of moderate activity or walking.",
                  "Keeping your body moving prevents many heart-related conditions.");

        addCategoryTitle("😋 Nutrition");
        addTipBox("Cut back on sugar, salt, and saturated fats.",
                  "Small reductions in salt or fat can lower blood pressure significantly.");

        addCategoryTitle("😴 Sleep & Stress");
        addTipBox("Stick to a regular sleep schedule.",
                  "Consistency in sleep promotes heart recovery and reduces stress.");
        addTipBox("Incorporate light mindfulness and relaxation into your day.",
                  "Simple habits like breathing or meditation can reduce your risk.");

        addCategoryTitle("😷 Monitoring");
        addTipBox("Go for routine checkups on blood pressure and cholesterol.",
                  "You can’t manage what you don’t measure. Stay informed!");
    }
    else {
        addCategoryTitle("😋 Nutrition");
        addTipBox("You're doing well! Keep eating balanced meals daily.",
                  "A variety of whole foods keeps your heart nourished and happy.");
        addTipBox("Stick to fruits, vegetables, and whole grains.",
                  "These foods are high in fiber and keep your arteries clean.");

        addCategoryTitle("😅 Activity");
        addTipBox("Stay active 30+ mins daily with light to moderate workouts.",
                  "Regular movement helps reduce the risk of future complications.");

        addCategoryTitle("😴 Sleep & Stress");
        addTipBox("Keep your sleep consistent and drink enough water.",
                  "Hydration and sleep support overall wellness and mental clarity.");

        addCategoryTitle("😷 Monitoring");
        addTipBox("Get occasional health screenings even if you feel well.",
                  "Preventive care helps catch issues before they become serious.");
    }

    scrollContent->setLayout(scrollLayout);
    scroll->setWidget(scrollContent);
    scroll->setStyleSheet("background: transparent;");
    mainLayout->addWidget(scroll);

    // Back button now uses the stored previousScreen pointer.
    QPushButton *backBtn = new QPushButton("← Back", this);
    backBtn->setFixedSize(200, 50);
    backBtn->setStyleSheet("background-color: white; color: #551900; border: 2px solid #551900; "
                           "border-radius: 15px; font-size: 16px; font-weight: bold;");
    connect(backBtn, &QPushButton::clicked, [=]() {
        if (previousScreen) {
            stackedWidget->setCurrentWidget(previousScreen);
            // Optionally, you can remove this TipsForUser widget from the stack:
            // stackedWidget->removeWidget(this);
        }
    });

    mainLayout->addWidget(backBtn, 0, Qt::AlignLeft);

    setLayout(mainLayout);
}

/**
 * @brief Paints the custom gradient background.
 *
 * Overrides the QWidget::paintEvent to fill the widget's background with a linear gradient that transitions
 * from color "#532e63" at the top to "#98002e" at the bottom.
 *
 * @param event Pointer to the QPaintEvent.
 */

void TipsForUser::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0.0, QColor("#532e63"));
    gradient.setColorAt(1.0, QColor("#98002e"));
    painter.fillRect(rect(), gradient);
    QWidget::paintEvent(event);
}
