#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonParseError>
#include <QJsonObject>
#include <QUrlQuery>

struct UserData{
    QString email;
    QString pass;
    QString path;
    bool active = false;
};

struct MessageProcess {
    QString msg;
    bool check = true;
    int error = 0;
};


class Network : public QObject
{
public:
    Network(QObject *parent = nullptr);
private:
protected:
    enum TYPE {GET, POST, DELETE, PUT};
    UserData userData;
    MessageProcess msgProc;
    QJsonParseError jError;
    QNetworkAccessManager *netMan;

    QByteArray requestTypeJson(QByteArray &packJson, QString url, TYPE type);
    QByteArray updateTicket(QByteArray &packJson, int id);

    QByteArray createTicket(QByteArray array);
    QByteArray getTickets();
    QByteArray getTicketsRange(QString date_start, QString date_end, bool sort_asc = false);
    QByteArray getServerData(QNetworkRequest req, TYPE t, QByteArray postData = "");


};

#endif // NETWORK_H
