/**
 * @file main.cpp
 * @brief Main entry point of the application.
 *
 * This file contains the main function which initializes the QApplication, creates and shows
 * the MainWindow, and starts the application's event loop.
 *
 * @return int Application exit status.
 * @author Ola Waked
 */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
