#ifndef MESSAGE_H
#define MESSAGE_H

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include <QJsonObject>
#include <QJsonDocument>
#include <QPair>
#include <QDateTime>
#include <QTime>

class Message
{
private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QNetworkReply *response;
    QUrl url;
    QJsonObject json;
    QJsonObject settings;
    QDateTime last_send;

    bool analize();
public slots:
    void confirm();
public:
    Message();
    void send_message(QString message, QPair<double, double> ubication);
};

#endif // MESSAGE_H
