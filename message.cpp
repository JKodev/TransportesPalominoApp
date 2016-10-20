#include "message.h"
#include <QFile>
#include <QString>
#include <iostream>

using namespace std;

bool Message::analize()
{
    QDateTime now_dt(QDateTime::currentDateTime());

    QTime last = this->last_send.time();
    QTime now = now_dt.time();

    if (last.hour() == now.hour()) {
        if (last.minute() == now.minute()) {
            return false;
        }
        if(now.minute() - last.minute() >= 1) {
            return true;
        }
    } else {
        return true;
    }
    return false;
}

Message::Message()
{
    QString string = "Tuesday, 23 April 12 22:51:41";
    QString format = "dddd, d MMMM yy hh:mm:ss";
    this->last_send = QDateTime::fromString(string, format);

    QString str_settings;
    QFile file;

    file.setFileName( "../settings.json" );
    file.open( QIODevice::ReadOnly | QIODevice::Text );

    str_settings = file.readAll();

    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(str_settings.toUtf8());
    this->settings = doc.object();

    QJsonValue structure = this->settings.value(QString("structure"));

    this->json = structure.toObject();

    this->manager = new QNetworkAccessManager;
    QJsonValue url = this->settings.take("url");

    this->url = QUrl(QString(url.toString()));

    this->request = QNetworkRequest(this->url);
    this->request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
}

void Message::send_message(QString message, QPair<double, double> ubication)
{
    if (!analize()) {
        return;
    }
    this->last_send = QDateTime::currentDateTime();

    QDateTime current(QDateTime::currentDateTime());
    this->json["when"] = current.toString("yyyy-MM-dd HH:mm:ss t");
    this->json["message"] = QJsonValue(message);
    this->json["latitude"] = QString::number(ubication.first);
    this->json["longitude"] = QString::number(ubication.second);

    QJsonDocument doc;
    doc.setObject(this->json);

    QByteArray request_body = doc.toJson();

    this->manager->post(request, request_body);
    cout << "---------------------------------------------------" << endl;
    cout << "\t SE HA ENVIADO UN MENSAJE AL SUPERVISOR." << endl;
    cout << "---------------------------------------------------" << endl;
}
