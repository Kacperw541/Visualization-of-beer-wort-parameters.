/*!
 * @file databasehandler.h
 * @author Kacper Wiśniewski (kwisniewski541@gmail.com)
 * @version 1.0
 * @date 2023-01-30
 *
 *  The DatabaseHandler class retrievs data from the Firebase database,
 *  analyzes it and send a signal with information about the data.
 */

#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

/*!
 * \brief The DatabaseHandler class - Retrieves data from the database and analyzes it.
 */
class DatabaseHandler : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseHandler(QObject *parent = nullptr);    ///< Construct a new DatabaseHandler object.
    ~DatabaseHandler();                                     ///< Destroy the DatabaseHandler object.

signals:
    void dataReady(QMap<QString, QVector <double>> &data);  ///< Signal emitted after successful data parsing.
    void readingDataError();                                ///< Signal emitted after unsuccessful data parsing.
    void dataEmpty();                                       ///< Signal emitted if received data is empty.
public slots:
    void networkReplyReadyRead();                       ///< Network reply read slot.
    void userSignedIn(const QString &endPoint);         ///< Recives information about successful user login.
    void refreshData();                                 ///< Supports a data refresh request.
private:
    bool parseResponse(const QByteArray &response);     ///< Analyzes and handles the response
    QMap<QString, QVector <double>> data;               ///< QMap with ready-to-use data.
    QString m_endPoint;                                 ///< Database endpoint
    QNetworkAccessManager *m_networkManager;
    QNetworkReply *m_networkReply;
};

#endif // DATABASEHANDLER_H
