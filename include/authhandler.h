/*!
 * @file authhandler.h
 * @author Kacper Wiśniewski (kwisniewski541@gmail.com)
 * @version 1.0
 * @date 2023-01-30
 *
 *  The AuthHandler class authorizes the user in the Firebase database.
 *  Handles user login to the database.
 *  Contains signals that are emitted after successful or unsuccessful user login.
 */

#ifndef AUTHHANDLER_H
#define AUTHHANDLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QVariantMap>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QMessageBox>
#include <QDebug>


/*!
 * \brief The AuthHandler class - authorizes the user in the database
 */
class AuthHandler : public QObject
{
    Q_OBJECT
public:
    explicit AuthHandler(QObject *parent = nullptr);   ///< Construct a new AuthHandler object.
    ~AuthHandler();                                    ///< Destroy the AuthHandler object.
    void setAPIKey(const QString &apiKey);             ///< Set database API key.
    void signUserIn(const QString &emailAddress, const QString &password); ///< Sign user in with email and password.
public slots:
    void networkReplyReadyRead();               ///< Network Reply read slot.
signals:
    void userSignedIn(const QString &endPoint); ///< Signal emitted after a successful user login.
    void userLoginFailed();                     ///< Signal emitted after a unsuccessful user login
private:
    void performPOST(const QString &url, const QJsonDocument &payload); ///< Network request, POST method.
    void parseResponse(const QByteArray &response);                     ///< Parse response and emit userSignedIn signal when response is correct
    QNetworkAccessManager *m_networkAccessManager;
    QNetworkReply *m_networkReply;
    QString m_apiKey;
    QString m_idToken;
    QString m_userID;
};

#endif // AUTHHANDLER_H
