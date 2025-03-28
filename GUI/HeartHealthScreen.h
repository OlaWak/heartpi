#ifndef HEARTHEALTHSCREEN_H
#define HEARTHEALTHSCREEN_H

/**
 * @file HeartHealthScreen.h
 * @brief Declaration of the HeartHealthScreen widget.
 *
 * This file declares the HeartHealthScreen class, which displays heart health results,
 * live heart rate charts, and historical data to the user. It integrates with family health
 * data and calculations to assess heart risk and provides visual and audio feedback based on
 * the risk level.
 *
 * The widget is designed to be used within a QStackedWidget for smooth screen transitions.
 *
 * @note It inherits from CustomBackgroundWidget to allow for custom background painting.
 *
 * @author Ola Waked
 */

#include "custombackgroundwidget.h"
#include "../FamilyHealth.h"
#include "../Calculations.h"
#include <QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QStackedWidget>
#include <QTimer>
#include <QDateTime>
#include <QStringList>
#include <QString>

using namespace QtCharts;

/**
 * @class HeartHealthScreen
 * @brief A widget that displays heart health results and a live heart rate chart.
 *
 * The HeartHealthScreen class provides a user interface that displays the results of a heart health assessment,
 * including simulated sensor readings, risk assessment messages, and a live updating heart rate chart.
 * It also supports data logging from CSV files and can play audio alerts based on the risk level.
 */

class HeartHealthScreen : public CustomBackgroundWidget
{
    Q_OBJECT

public:

    /**
     * @brief Constructs a new HeartHealthScreen object.
     *
     * @param stackedWidgetRef Pointer to the QStackedWidget used for navigating between screens.
     * @param username The username of the current user.
     * @param parent Pointer to the parent widget (default is nullptr).
     */
    explicit HeartHealthScreen(QStackedWidget *stackedWidgetRef,
                               const QString &username,
                               QWidget *parent = nullptr);

    
    /**
     * @brief Displays the heart health results after the user completes the survey.
     *
     * This method uses the provided FamilyHealth object to assess the heart health and update
     * the UI with simulated sensor readings and risk assessment.
     *
     * @param familyData The FamilyHealth object containing the user's health data.
     */
    void displayResults(const FamilyHealth &familyData);

    /**
     * @brief Sets the current username.
     *
     * Updates the username, which is used for data logging and display.
     *
     * @param username The new username.
     */

    void setUser(const QString &username) { user = username; }

    /**
     * @brief Resets all UI elements and internal data to their initial state.
     *
     * Clears labels, chart data, timers, and any stored historical data.
     */
    void reset();

public slots:
    
    /**
     * @brief Slot to navigate back to the main menu.
     *
     * This slot is triggered, for example, by a back button press to return to the main menu screen.
     */
    void goBackToMain();

    /**
     * @brief Slot to display previous results for the user from CSV data.
     *
     * This slot reloads historical data from a CSV file and updates the UI with past heart rate results.
     */
    void showResultsForUser(); // new slot to show results based on CSV

signals:
     /**
     * @brief Signal emitted when navigating back to the main menu.
     */
    void backToMain();

protected:
     /**
     * @brief Paints a custom background based on the risk level.
     *
     * Overrides the QWidget::paintEvent to draw different background gradients or colors depending
     * on the current risk (e.g., moderate risk shows a gradient).
     *
     * @param event Pointer to the QPaintEvent.
     */
    void paintEvent(QPaintEvent *event) override;

private slots:
     /**
     * @brief Updates the live heart rate chart.
     *
     * Called automatically by a QTimer to append new heart rate data to the chart in real time.
     */
    void updateLiveChart();

private:
    /**
     * @brief Sets up the user interface components.
     *
     * Initializes labels, chart view, buttons, and layouts.
     */
    void setupUI();

    /**
     * @brief Configures the live heart rate chart.
     *
     * Initializes the QChart, QLineSeries, and chart axes, and starts the timer for live updates.
     */
    void setupLiveChart();

    /**
     * @brief Generates a heart rate chart (not used for live updates).
     *
     * @param heartRate The heart rate value (unused).
     */
    void generateHeartRateChart(double heartRate); // not used for live updates

     /**
     * @brief Loads historical heart rate data from a CSV file.
     *
     * Reads the CSV file and extracts heart rate values associated with the current user for display.
     *
     * @param filename The name of the CSV file to load.
     */
    void loadDataFromCSV(const QString &filename);
    QTimer *m_beepTimer = nullptr;     ///< Timer for playing audio alerts based on risk.
    QStringList liveDataLines;         ///< Stores live heart rate data lines from CSV.
    QStackedWidget *stackedWidget;     ///< Pointer to the main QStackedWidget for screen navigation.
    QString user;                       ///< Current user's name.

    // Widgets
    QLabel      *resultLabel;
    QLabel      *heartRateLabel;
    QChartView  *chartView;
    QLineSeries *heartRateSeries;
    QChart      *chart;
    QTimer      *liveTimer;
    
    int currentX = 0;
    int liveDataIndex = 0;

    // Used to indicate the risk category (e.g., "moderate" to paint a gradient)
    QString m_currentRisk;
};

#endif // HEARTHEALTHSCREEN_H
