/**
 * @file EmailSender.cpp
 * @brief Implements the EmailSender class for sending emails via SMTP.
 *
 * This file defines the sendEmail() method which establishes an SSL connection to the Gmail SMTP server,
 * authenticates using credentials provided via environment variables, and sends an email with the given
 * subject and body.
 *
 * @note Ensure that the environment variables HEARTPI_EMAIL and HEARTPI_APP_PASSWORD are set before running this code.
 * 
 * @author Ola Waked
 */

#include "EmailSender.h"
#include <QSslSocket>
#include <QDebug>
#include <QByteArray>


/**
 * @brief Sends an email using SMTP over an SSL connection.
 *
 * This function reads the sender's email and application password from environment variables,
 * establishes an SSL connection to Gmail's SMTP server on port 465, and performs the necessary
 * SMTP commands (EHLO, AUTH LOGIN, MAIL FROM, RCPT TO, DATA, QUIT) to send an email.
 *
 * A helper lambda function is used to send each SMTP command and log the corresponding responses.
 *
 * @param[in] to The recipient's email address.
 * @param[in] subject The subject of the email.
 * @param[in] body The body content of the email.
 * @return true if the email was sent successfully; false if any step fails.
 */

bool EmailSender::sendEmail(const QString &to, const QString &subject, const QString &body)
{
    // Read email credentials from environment variables.
    // These were set on my system for security as the repo is public
    // HEARTPI_EMAIL and HEARTPI_APP_PASSWORD
    QString senderEmail = QString::fromUtf8(qgetenv("HEARTPI_EMAIL"));
    QString appPassword   = QString::fromUtf8(qgetenv("HEARTPI_APP_PASSWORD"));

    if (senderEmail.isEmpty() || appPassword.isEmpty()) {
        qDebug() << "Missing environment variables for email!";
        return false;
    }

    QSslSocket socket;
    socket.connectToHostEncrypted("smtp.gmail.com", 465);
    if (!socket.waitForConnected(5000)) {
        qDebug() << "Connection to SMTP server failed!";
        return false;
    }

    // Helper lambda: sends a command and reads the response.
    auto sendCmd = [&](const QString &text) -> QString {
        socket.write(text.toUtf8() + "\r\n");
        socket.flush();
        if (!socket.waitForBytesWritten(1000)) {
            qDebug() << "Failed to write:" << text;
        }
        if (!socket.waitForReadyRead(5000)) {
            qDebug() << "No response for:" << text;
        }
        QString response = QString::fromUtf8(socket.readAll());
        qDebug() << "Sent:" << text;
        qDebug() << "Response:" << response;
        return response;
    };

    // Begin SMTP communication.
    sendCmd("EHLO heartpi");
    sendCmd("AUTH LOGIN");

    // Base64 encode credentials.
    QString base64Email    = senderEmail.toUtf8().toBase64();
    QString base64Password = appPassword.toUtf8().toBase64();

    sendCmd(base64Email);
    sendCmd(base64Password);

    sendCmd("MAIL FROM:<" + senderEmail + ">");
    sendCmd("RCPT TO:<" + to + ">");
    sendCmd("DATA");

    // Construct email message with headers.
    QString message = "Subject: " + subject + "\r\n"
                      "To: " + to + "\r\n"
                      "From: " + senderEmail + "\r\n"
                      "MIME-Version: 1.0\r\n"
                      "Content-Type: text/plain; charset=UTF-8\r\n\r\n" +
                      body + "\r\n.\r\n";

    sendCmd(message);
    sendCmd("QUIT");

    socket.disconnectFromHost();
    return true;
}
