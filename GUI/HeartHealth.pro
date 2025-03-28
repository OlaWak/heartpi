QT       += core gui widgets charts multimedia network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HeartHealthGUI
TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp \
           SurveyScreen.cpp \
           SurveyFormScreen.cpp \
           custombackgroundwidget.cpp \
           HeartHealthScreen.cpp \
           ResultsLoginScreen.cpp \
           WelcomeScreen.cpp \
           TipsForUser.cpp \
           EmailSender.cpp \
           NotifyCaregiverScreen.cpp \
           ../Calculations.cpp \
           ../FamilyHealth.cpp \
           ../RandomNumberGenerator.cpp

HEADERS += mainwindow.h \
           custombackgroundwidget.h \
           SurveyScreen.h \
           SurveyFormScreen.h \
           HeartHealthScreen.h \
           WelcomeScreen.h \
           ResultsLoginScreen.h \
           TipsForUser.h \
           EmailSender.h \
           NotifyCaregiverScreen.h \
           ../Calculations.h \
           ../FamilyHealth.h \
           ../RandomNumberGenerator.h

# Ensure the images folder is included during deployment
DISTFILES += images/bg.png images/heartpilogo.png
