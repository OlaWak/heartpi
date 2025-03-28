/**
 * @file HeartHealthScreen.cpp
 * @brief Implements the HeartHealthScreen widget for displaying heart health results and live charts.
 *
 * This file defines the HeartHealthScreen class methods, which provide a user interface to display heart health
 * assessments, live heart rate charts, and historical data. The widget uses Qt Widgets and Qt Charts, integrates with
 * the Calculations and RandomNumberGenerator modules, and provides visual and audio feedback based on risk levels.
 *
 * @note The widget is designed to be used within a QStackedWidget for screen transitions.
 *
 * @author Ola Waked
 */

#include "HeartHealthScreen.h"
#include "../RandomNumberGenerator.h"
#include "../Calculations.h" // For assessHeartHealth()
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QApplication>
#include <QPalette>
#include <QSound>
#include <QTimer>
#include <QDebug>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>
#include <QSoundEffect>
#include <QUrl>


/**
 * @brief Constructs a new HeartHealthScreen object.
 *
 * Initializes the HeartHealthScreen with the provided QStackedWidget reference, username, and parent widget.
 * The constructor sets widget attributes for painting and then calls setupUI() to initialize the user interface.
 *
 * @param stackedWidgetRef Pointer to the QStackedWidget managing the application's screens.
 * @param username The username of the current user.
 * @param parent Pointer to the parent QWidget.
 */

HeartHealthScreen::HeartHealthScreen(QStackedWidget *stackedWidgetRef,
                                     const QString &username,
                                     QWidget *parent)
    : CustomBackgroundWidget(parent),
      stackedWidget(stackedWidgetRef),
      user(username),
      chart(nullptr),
      liveTimer(nullptr),
      currentX(0),
      liveDataIndex(0),
      m_currentRisk("")
{
    // Force our paint event to be the only one (opaque background)
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_StyledBackground, true);
    setupUI();
}

/**
 * @brief Sets up the user interface components.
 *
 * Initializes UI elements such as labels, chart view, and back button. Sets layouts and connects signals
 * to appropriate slots.
 */

void HeartHealthScreen::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);

    // Result label (smaller font)
    resultLabel = new QLabel(this);
    resultLabel->setStyleSheet("color: white; font-size: 16px; font-weight: bold;");
    resultLabel->setAlignment(Qt::AlignCenter);

    // Heart rate label (smaller than before)
    heartRateLabel = new QLabel(this);
    heartRateLabel->setStyleSheet("color: white; font-size: 32px; font-weight: bold;");
    heartRateLabel->setAlignment(Qt::AlignCenter);
    heartRateLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // Chart view: let it expand to take available space.
    chartView = new QChartView(this);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Back button setup.
    QPushButton *backButton = new QPushButton("Back to Main Menu", this);
    QString buttonStyle = "QPushButton {"
                          "  background-color: white;"
                          "  color: #551900;"
                          "  border: 2px solid #551900;"
                          "  border-radius: 20px;"
                          "  font-size: 16px;"
                          "  font-weight: bold;"
                          "  padding: 8px 16px;"
                          "  min-width: 250px;"
                          "  font-family: 'Poppins', sans-serif;"
                          "}";
    backButton->setStyleSheet(buttonStyle);
    backButton->setFixedSize(300, 60);
    connect(backButton, &QPushButton::clicked, this, &HeartHealthScreen::goBackToMain);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch();

    layout->addWidget(resultLabel, 0);
    layout->addWidget(heartRateLabel, 0);
    layout->addWidget(chartView, 15);
    layout->addLayout(buttonLayout, 0);

    setLayout(layout);
    setupLiveChart();
}

/**
 * @brief Initializes the live heart rate chart.
 *
 * Sets up a QChart with a QLineSeries to display live heart rate data. Configures the chart appearance,
 * axes, and connects a QTimer to update the chart at regular intervals.
 */

void HeartHealthScreen::setupLiveChart()
{
    chart = new QChart();
    heartRateSeries = new QLineSeries();

    QPen redPen(Qt::red);
    redPen.setWidth(2);
    heartRateSeries->setPen(redPen);
    chart->addSeries(heartRateSeries);

    chart->setBackgroundBrush(QBrush(Qt::black));              // Black chart background
    chart->setPlotAreaBackgroundBrush(QBrush(Qt::black));      // Plot area also black
    chart->setPlotAreaBackgroundVisible(true);                 // Make it visible
    chartView->setStyleSheet("background-color: black; border: none;");

    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, 50);
    axisX->setLabelFormat("%.0f");
    axisX->setTitleText("Time (sec)");
    axisX->setLabelsColor(Qt::white);
    axisX->setTitleBrush(QBrush(Qt::white));

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(50, 130);
    axisY->setTitleText("BPM");
    axisY->setLabelsColor(Qt::white);
    axisY->setTitleBrush(QBrush(Qt::white));

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    heartRateSeries->attachAxis(axisX);
    heartRateSeries->attachAxis(axisY);

    chart->setTitle("Live Heart Rate Monitor");
    QFont titleFont;
    titleFont.setBold(true);
    titleFont.setPointSize(14);
    chart->setTitleFont(titleFont);
    chart->setTitleBrush(Qt::white);

    chartView->setChart(chart);

    liveTimer = new QTimer(this);
    connect(liveTimer, &QTimer::timeout, this, &HeartHealthScreen::updateLiveChart);
    liveTimer->start(1000);
}


/**
 * @brief Custom paint event to update the background based on risk level.
 *
 * Overrides the QWidget paintEvent to draw different background gradients or colors depending on the
 * current risk level (low, moderate, high).
 *
 * @param event Pointer to the QPaintEvent.
 */
void HeartHealthScreen::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (m_currentRisk == "moderate") {
        QLinearGradient gradient(0, 0, 0, height());
        gradient.setColorAt(0.0, QColor("#532e63"));
        gradient.setColorAt(1.0, QColor("#98002e"));
        painter.fillRect(rect(), gradient);
    } else if (m_currentRisk == "low") {
        painter.fillRect(rect(), QColor("#00FF00"));
    } else if (m_currentRisk == "high") {
        painter.fillRect(rect(), QColor("#FF0000"));
    } else {
        painter.fillRect(rect(), palette().window());
    }
    QWidget::paintEvent(event);
}

/**
 * @brief Displays heart health results and updates historical data.
 *
 * Computes the heart health assessment using family data and updates UI elements with the simulated sensor readings.
 * Adjusts the background color and starts a beeping timer if the risk is high. Also updates the CSV file with simulated
 * heart rate data.
 *
 * @param familyData The FamilyHealth object containing user data.
 */
void HeartHealthScreen::displayResults(const FamilyHealth &familyData)
{
    double heartRate, sysBP, diasBP, cholesterol, ecg;
    std::string assessment = assessHeartHealth(familyData,
                                               heartRate,
                                               sysBP,
                                               diasBP,
                                               cholesterol,
                                               ecg);

    resultLabel->setText(QString::fromStdString(assessment));
    heartRateLabel->setText("Heart Rate: " + QString::number(heartRate, 'f', 1) + " BPM");

    if (assessment.find("Low risk of heart disease.") != std::string::npos) {
        m_currentRisk = "low";
    } else if (assessment.find("High risk of heart disease.") != std::string::npos) {
        m_currentRisk = "high";
        // Stop any existing timer first
        if (m_beepTimer) {
            m_beepTimer->stop();
            m_beepTimer->deleteLater();
        }
        
        m_beepTimer = new QTimer(this);
        connect(m_beepTimer, &QTimer::timeout, this, [=]() {
            //qDebug() << "Attempting to play sound...";  // Debug output
            QSoundEffect* sound = new QSoundEffect(this);
            sound->setSource(QUrl::fromLocalFile("sounds/beep.wav"));
            sound->setVolume(1.0f);
            sound->play();
            // Clean up the sound effect after it's done playing
            QTimer::singleShot(1000, sound, &QObject::deleteLater);
        });
        m_beepTimer->start(2000);  // Play every 2 seconds
    } else if (assessment.find("Moderate risk of heart disease.") != std::string::npos) {
        m_currentRisk = "moderate";
    } else {
        m_currentRisk = "";
    }

    if (!user.isEmpty()) {
        QFile file("userdata.csv");
        QStringList lines;
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd())
                lines.append(in.readLine());
            file.close();
        }
        if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            QTextStream out(&file);
            for (const QString &line : lines)
                out << line << "\n";
            for (int i = 0; i < 20; i++) {
                qint64 now = QDateTime::currentSecsSinceEpoch() + i;
                double simulatedHR = RandomNumberGenerator(heartRate - 5, heartRate + 5).generate();
                QString newLine = user + "," + QString::number(now) + "," + QString::number(simulatedHR, 'f', 1);
                out << newLine << "\n";
            }
            file.close();
        }
    }
    loadDataFromCSV("userdata.csv");
    update();
}

/**
 * @brief Resets the HeartHealthScreen to its initial state.
 *
 * Clears UI labels, resets internal data used for live chart updates and historical data, and stops
 * any active beeping timers.
 */

void HeartHealthScreen::reset()
{
    // Stop the beeping timer if it exists
    if (m_beepTimer) {
        m_beepTimer->stop();
        m_beepTimer->deleteLater();
        m_beepTimer = nullptr;
    }

    resultLabel->clear();
    heartRateLabel->clear();
    m_currentRisk = "";
    if (heartRateSeries)
        heartRateSeries->clear();
    currentX = 0;
    liveDataIndex = 0;
    liveDataLines.clear();
}

/**
 * @brief (Unused) Placeholder for generating a heart rate chart.
 *
 * This function is currently not used for live updates.
 *
 * @param heartRate The heart rate value (unused).
 */
void HeartHealthScreen::generateHeartRateChart(double /*heartRate*/)
{
    // Not used for live updates.
}


/**
 * @brief Updates the live heart rate chart.
 *
 * Retrieves the next heart rate value from liveDataLines (if available) or generates a random value,
 * then appends the new data point to the chart series. Scrolls the chart if necessary.
 */
void HeartHealthScreen::updateLiveChart()
{
    double newHeartRate = 70.0;
    if (!liveDataLines.isEmpty() && liveDataIndex < liveDataLines.size()) {
        bool ok = false;
        double val = liveDataLines[liveDataIndex].toDouble(&ok);
        if (ok)
            newHeartRate = val;
        liveDataIndex++;
    } else {
        newHeartRate = RandomNumberGenerator(60, 100).generate();
    }
    heartRateSeries->append(currentX, newHeartRate);
    currentX++;
    if (currentX > 50)
        chart->scroll(chart->plotArea().width() / 50.0, 0);
}

/**
 * @brief Loads heart rate data from a CSV file.
 *
 * Reads the CSV file, extracts heart rate data associated with the current user, and stores it for use
 * in live chart updates.
 *
 * @param filename The name of the CSV file to read.
 */

void HeartHealthScreen::loadDataFromCSV(const QString &filename)
{
    liveDataLines.clear();
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Could not open CSV file.");
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty())
            continue;
        QStringList parts = line.split(",");
        if (parts.size() == 3) {
            QString lineUser = parts[0];
            QString hrStr = parts[2];
            if (lineUser == user)
                liveDataLines.append(hrStr);
        }
    }
    file.close();
    liveDataIndex = 0;
}

/**
 * @brief Navigates back to the main menu.
 *
 * Stops any active timers and instructs the QStackedWidget to switch to the main menu screen.
 * Emits the backToMain signal.
 */
void HeartHealthScreen::goBackToMain()
{
    // Stop the beeping timer if it exists
    if (m_beepTimer) {
        m_beepTimer->stop();
        m_beepTimer->deleteLater();
        m_beepTimer = nullptr;
    }

    if (stackedWidget)
        stackedWidget->setCurrentIndex(0);
    emit backToMain();
}

/**
 * @brief Displays previous heart health results for the current user.
 *
 * Clears current data and reloads historical data from the CSV file to display previous results.
 */
void HeartHealthScreen::showResultsForUser() {
    resultLabel->setText("Previous Results for " + user);
    heartRateLabel->clear();
    if (heartRateSeries)
        heartRateSeries->clear();
    currentX = 0;
    liveDataIndex = 0;
    liveDataLines.clear();
    loadDataFromCSV("userdata.csv");
    update();
}
