#ifndef SURVEYFORMSCREEN_H
#define SURVEYFORMSCREEN_H


/**
 * @file SurveyFormScreen.h
 * @brief Declaration of the SurveyFormScreen widget.
 *
 * This file declares the SurveyFormScreen class, which provides a user interface for collecting
 * heart health survey data. It displays a series of questions using combo boxes, collects the
 * responses, and packages the data into a FamilyHealth object. Once the survey is submitted,
 * a signal is emitted containing the collected data.
 *
 * @note This widget is intended to be used within a QStackedWidget for screen navigation.
 *
 * @author Ola Waked
 */
#include "custombackgroundwidget.h"
#include "../FamilyHealth.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QStackedWidget>

/**
 * @class SurveyFormScreen
 * @brief A widget for collecting heart health survey data.
 *
 * The SurveyFormScreen class displays survey questions related to the user's health, such as age,
 * gender, sleep hours, exercise frequency, family health history, diet type, and smoking status.
 * After the user completes the survey, the widget emits a signal with the collected FamilyHealth data.
 */

class SurveyFormScreen : public CustomBackgroundWidget
{
    Q_OBJECT

public:
    explicit SurveyFormScreen(QStackedWidget *stackedWidgetRef,
                              const QString &username,
                              QWidget *parent = nullptr);

    // Reset all survey form fields to initial state
    void reset();

signals:
    void surveyCompleted(const FamilyHealth &familyData);

private slots:
    void submitSurvey();

private:
    void setupUI();

    FamilyHealth      familyData;
    QVector<QComboBox*> questionFields;
    QStackedWidget   *stackedWidget;

    // Field to hold 'username' if needed later
    QString user;
};

#endif // SURVEYFORMSCREEN_H
