/**
 * @file authhandler.cpp
 * @author Kacper Wiśniewski (kwisniewski541@gmail.com)
 * @version 1.0
 * @date 2023-01-30
 */

#include "authhandler.h"

/*!
 * \brief AuthHandler::AuthHandler - Construct a new AuthHandler object.
 * \param parent - pointer to the parent object.
 */
AuthHandler::AuthHandler(QObject *parent)
    : QObject{parent}
    , m_apiKey(QString())
{
    m_networkAccessManager = new QNetworkAccessManager(this);
}

/*!
 * \brief AuthHandler::~AuthHandler - Destroy the AuthHandler object.
 */
AuthHandler::~AuthHandler()
{
    m_networkAccessManager->deleteLater();
}

/*!
 * \brief AuthHandler::setAPIKey - Set database web API key.
 * \param apiKey - database web API key to set.
 */
void AuthHandler::setAPIKey(const QString &apiKey)
{
    m_apiKey = apiKey;
}

/*!
 * \brief AuthHandler::signUserIn - Sign user in with email and password.
 * \param emailAddress - User email address.
 * \param password - User password.
 */
void AuthHandler::signUserIn(const QString &emailAddress, const QString &password)
{
    QString signInEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + m_apiKey;

    QVariantMap variantPayload;
    variantPayload["email"] = emailAddress;
    variantPayload["password"] = password;
    variantPayload["returnSecureToken"] = true;

    QJsonDocument jsonPayload = QJsonDocument::fromVariant(variantPayload);
    performPOST(signInEndpoint, jsonPayload);
}

/*!
 * \brief AuthHandler::networkReplyReadyRead - Reply read slot.
 */
void AuthHandler::networkReplyReadyRead()
{
    QByteArray response = m_networkReply->readAll();
    m_networkReply->deleteLater();
    parseResponse(response);
}

/*!
 * \brief AuthHandler::performPOST -  Network request, POST method.
 * \param url - SignIn end point.
 * \param payload - Payload with request body.
 */
void AuthHandler::performPOST(const QString &url, const QJsonDocument &payload)
{
    QNetworkRequest newRequest((QUrl(url)));
    newRequest.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));

    m_networkReply = m_networkAccessManager->post(newRequest, payload.toJson());
    connect(m_networkReply, &QNetworkReply::readyRead, this, &AuthHandler::networkReplyReadyRead);
}

/*!
 * \brief AuthHandler::parseResponse - Parse response and emit userSignedIn signal when response is correct,
 *                                     otherwise emit userLoginFailed signal.
 * \param response - Received network response.
 */
void AuthHandler::parseResponse(const QByteArray &response)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(response);
    if (jsonDocument.object().contains("error"))
    {
        qDebug() << "Error occured!" << response;
        emit userLoginFailed();
    }
    else if (jsonDocument.object().contains("kind"))
    {
        m_idToken = jsonDocument.object().value("idToken").toString();
        m_userID = jsonDocument.object().value("localId").toString();
        qDebug() << "User signed in successfully!";
        QString endPoint = "https://beer-wort-parameter-monitoring-default-rtdb.europe-west1.firebasedatabase.app/UsersData/"
                            + m_userID + "/readings.json?auth=" + m_idToken;

        emit userSignedIn(endPoint);
    }
}
