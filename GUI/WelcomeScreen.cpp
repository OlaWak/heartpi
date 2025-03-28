#include "WelcomeScreen.h"
#include "TipsForUser.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QFrame>
#include <QPushButton>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
using namespace QtCharts;

/**
 * @file WelcomeScreen.cpp
 * @brief Implements the WelcomeScreen widget which displays a personalized welcome message,
 * user heart rate statistics, and a historical chart.
 *
 * The WelcomeScreen class reads the user's heart rate data from a CSV file ("userdata.csv"),
 * calculates statistics such as the average and latest heart rate, and determines the user's risk level.
 * It also provides navigation buttons, including one to display tailored health tips via the TipsForUser widget.
 *
 * @note This widget is designed to be used within a QStackedWidget for screen navigation.
 *
 * @author Ola Waked
 */


 /**
 * @brief Constructs a new WelcomeScreen object.
 *
 * Initializes the WelcomeScreen widget with a reference to the QStackedWidget for navigation,
 * the username of the current user, and sets up the UI components. The screen displays a welcome
 * message, statistical data from the user's heart rate records, and a historical chart.
 *
 * @param stackedWidgetRef Pointer to the QStackedWidget used for navigating between screens.
 * @param username The current user's username.
 * @param parent Pointer to the parent widget (default is nullptr).
 */
WelcomeScreen::WelcomeScreen(QStackedWidget *stackedWidgetRef,
                             const QString &username,
                             QWidget *parent)
    : CustomBackgroundWidget(parent),
      stackedWidget(stackedWidgetRef),
      user(username)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(40);

    // Styled info container
    QFrame *infoFrame = new QFrame(this);
    infoFrame->setStyleSheet("background-color: rgba(255, 255, 255, 0.1);"
                             "border-radius: 20px; padding: 30px;");
    QVBoxLayout *infoLayout = new QVBoxLayout(infoFrame);
    infoLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    infoLayout->setSpacing(20);

    QLabel *welcomeLabel = new QLabel("Welcome, " + user, this);
    welcomeLabel->setStyleSheet("color: white; font-size: 28px; font-weight: bold;");
    infoLayout->addWidget(welcomeLabel);

    // Read CSV for stats
    QVector<double> heartRates;
    qint64 latestTimestamp = 0;
    QFile file("userdata.csv");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        bool firstLine = true;
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (firstLine) { firstLine = false; continue; }
            QStringList parts = line.split(",");
            if (parts.size() == 3 && parts[0] == user) {
                bool ok;
                double bpm = parts[2].toDouble(&ok);
                if (ok) heartRates.append(bpm);
                latestTimestamp = parts[1].toLongLong();
            }
        }
        file.close();
    }

    QLabel *riskLabel = new QLabel(this);
    QLabel *averageLabel = new QLabel(this);
    QLabel *latestLabel = new QLabel(this);
    QLabel *timestampLabel = new QLabel(this);

    riskLabel->setStyleSheet("color: white; font-size: 20px;");
    averageLabel->setStyleSheet("color: white; font-size: 18px;");
    latestLabel->setStyleSheet("color: white; font-size: 18px;");
    timestampLabel->setStyleSheet("color: white; font-size: 16px; font-style: italic;");

    if (!heartRates.isEmpty()) {
        double sum = 0;
        for (double val : heartRates) sum += val;
        double avg = sum / heartRates.size();
        double latest = heartRates.last();

        QString risk;
        if (avg < 80) risk = "Low";
        else if (avg < 100) risk = "Moderate";
        else risk = "High";

        averageLabel->setText("Average Heart Rate: " + QString::number(avg, 'f', 1) + " BPM");
        latestLabel->setText("Latest Heart Rate: " + QString::number(latest, 'f', 1) + " BPM");
        riskLabel->setText("Risk Level: " + risk);

        if (latestTimestamp > 0) {
            QDateTime dt = QDateTime::fromSecsSinceEpoch(latestTimestamp);
            timestampLabel->setText("Last Reading: " + dt.toString("yyyy-MM-dd hh:mm:ss"));
        }
    } else {
        averageLabel->setText("No heart rate data found.");
    }

    infoLayout->addWidget(riskLabel);
    infoLayout->addWidget(averageLabel);
    infoLayout->addWidget(latestLabel);
    infoLayout->addWidget(timestampLabel);
    infoLayout->addStretch();

    QPushButton *tipsBtn = new QPushButton("Tips for You", this);
    tipsBtn->setFixedSize(300, 50);
    tipsBtn->setStyleSheet("background-color: white; color: #551900; border: 2px solid #551900; "
                           "border-radius: 15px; font-size: 16px; font-weight: bold;");
    connect(tipsBtn, &QPushButton::clicked, [=]() {
        // Create TipsForUser with the current WelcomeScreen (this) as the previous screen.
        TipsForUser *tipsScreen = new TipsForUser(stackedWidget,
                                                  riskLabel->text().section(':', 1).trimmed(),
                                                  this, // previous screen pointer
                                                  stackedWidget); // set parent to stackedWidget
        stackedWidget->addWidget(tipsScreen);
        stackedWidget->setCurrentWidget(tipsScreen);
    });
    infoLayout->addWidget(tipsBtn);
    
    QPushButton *backBtn = new QPushButton("\u2190 Back to Main Menu", this);
    backBtn->setFixedSize(300, 50);
    backBtn->setStyleSheet("background-color: white; color: #551900; border: 2px solid #551900; "
                           "border-radius: 15px; font-size: 16px; font-weight: bold;");
    connect(backBtn, &QPushButton::clicked, [=]() {
        if (stackedWidget) stackedWidget->setCurrentIndex(0);
    });
    infoLayout->addWidget(backBtn);

    // Create chart from CSV data
    QChart *chart = new QChart();
    QLineSeries *series = new QLineSeries();
    chart->addSeries(series);

    QPen redPen(Qt::red);
    redPen.setWidth(2);
    series->setPen(redPen);

    for (int i = 0; i < heartRates.size(); ++i) {
        series->append(i, heartRates[i]);
    }

    QValueAxis *axisX = new QValueAxis();
    // Fix: use int literal 50 so both arguments are int.
    axisX->setRange(0, qMax(heartRates.size(), 50));
    axisX->setTitleText("Time");
    axisX->setLabelsColor(Qt::white);
    axisX->setTitleBrush(QBrush(Qt::white));

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(50, 130);
    axisY->setTitleText("BPM");
    axisY->setLabelsColor(Qt::white);
    axisY->setTitleBrush(QBrush(Qt::white));

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    chart->setTitle("Preiviously Generated Heart Rate Monitor Chart");
    QFont titleFont;
    titleFont.setBold(true);
    titleFont.setPointSize(14);
    chart->setTitleFont(titleFont);
    chart->setTitleBrush(QBrush(Qt::white));

    chart->setBackgroundBrush(QBrush(Qt::black));
    chart->setPlotAreaBackgroundBrush(QBrush(Qt::black));
    chart->setPlotAreaBackgroundVisible(true);

    QChartView *chartView = new QChartView(chart);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartView->setStyleSheet("background-color: black; border: none;");

    mainLayout->addWidget(infoFrame, 1);
    mainLayout->addWidget(chartView, 2);
    setLayout(mainLayout);
}
