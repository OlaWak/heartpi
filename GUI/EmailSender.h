/**
 * @file EmailSender.h
 * @brief Declaration of the EmailSender class.
 *
 * This file declares the EmailSender class, which provides functionality to send emails using Gmail's SMTP server.
 * The class retrieves credentials from environment variables and sends an email with a specified recipient, subject, and body.
 *
 * @note Ensure that the environment variables HEARTPI_EMAIL and HEARTPI_APP_PASSWORD are set appropriately.
 * 
 * @author Ola Waked
 */

#ifndef EMAILSENDER_H
#define EMAILSENDER_H

#include <QString>

/**
 * @class EmailSender
 * @brief Provides functionality for sending emails via Gmail's SMTP server.
 *
 * The EmailSender class encapsulates the process of sending an email using Gmail's SMTP server over a secure SSL connection.
 * It uses credentials from environment variables to authenticate with the server and provides a static method to send an email.
 */

class EmailSender
{
public:
    /**
     * @brief Sends an email via Gmail’s SMTP server.
     *
     * This static function sends an email by establishing an SSL connection to Gmail's SMTP server. It reads the sender's
     * email address and application-specific password from environment variables, authenticates with the server, and sends
     * an email with the provided recipient, subject, and body. It returns true if the email appears to have been sent successfully.
     *
     * @param[in] to The recipient's email address.
     * @param[in] subject The subject of the email.
     * @param[in] body The body content of the email.
     * @return true if the email was sent successfully; false otherwise.
     */
    static bool sendEmail(const QString &to, const QString &subject, const QString &body);
};

#endif // EMAILSENDER_H
