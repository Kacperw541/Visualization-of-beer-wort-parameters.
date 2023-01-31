/**
 * @file databasehandler.cpp
 * @author Kacper Wiśniewski (kwisniewski541@gmail.com)
 * @version 1.0
 * @date 2023-01-30
 */

#include "databasehandler.h"

/*!
 * \brief DatabaseHandler::DatabaseHandler - Construct a new DatabaseHandler object.
 * \param parent - pointer to the parent object.
 */
DatabaseHandler::DatabaseHandler(QObject *parent)
    : QObject{parent}
{
    m_networkManager = new QNetworkAccessManager(this);
}

/*!
 * \brief DatabaseHandler::~DatabaseHandler - Destroy the DatabaseHandler object.s
 */
DatabaseHandler::~DatabaseHandler()
{
    m_networkManager->deleteLater();
}

/*!
 * \brief DatabaseHandler::networkReplyReadyRead - Reads the networkReply when it's ready.
 */
void DatabaseHandler::networkReplyReadyRead()
{
    QByteArray response = m_networkReply->readAll();
    m_networkReply->deleteLater();
    if (!parseResponse(response))
        refreshData();
}

/*!
 * \brief DatabaseHandler::userSignedIn - Recives information about successful user login.
 * \param endPoint - Firebase Database endpoint.
 */
void DatabaseHandler::userSignedIn(const QString &endPoint)
{
    m_endPoint = endPoint;
    m_networkReply = m_networkManager->get(QNetworkRequest(QUrl(m_endPoint)));
    connect(m_networkReply, &QNetworkReply::readyRead, this, &DatabaseHandler::networkReplyReadyRead);
}

/*!
 * \brief DatabaseHandler::refreshData - Supports a data refresh request.
 */
void DatabaseHandler::refreshData()
{
    m_networkReply = m_networkManager->get(QNetworkRequest(QUrl(m_endPoint)));
    connect(m_networkReply, &QNetworkReply::readyRead, this, &DatabaseHandler::networkReplyReadyRead);
}

/*!
 * \brief DatabaseHandler::parseResponse - Parse response and emit dataReady or dataEmpty signal when parsing passed,
 *                                         otherwise emit readingDatError signal.
 * \param response - Received network response.
 * \return true if successful, otherwise false.
 */
bool DatabaseHandler::parseResponse(const QByteArray &response)
{
    if (response.isEmpty())
    {
        emit dataEmpty();
        return true;
    }

    QJsonDocument jsonDocument = QJsonDocument::fromJson(response);
    if (jsonDocument.isEmpty() && !response.isEmpty())
        return false;

    if (jsonDocument.object().contains("error"))
        emit readingDataError();
    else
    {
        QJsonObject jsonObject = jsonDocument.object();
        foreach(const QString& param, jsonObject.keys())
        {
            QJsonObject param_obj = jsonObject.value(param).toObject();
            QVector <double> values;
            foreach(const QString& key, param_obj.keys())
                values << param_obj.value(key).toDouble();

            data.insert(param, values);
        }
        emit dataReady(data);
    }
    return true;
}
