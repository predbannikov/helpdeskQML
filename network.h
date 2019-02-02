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


class Network : public QObject
{
public:
    Network(QObject *parent = nullptr);
private:
protected:
    enum TYPE {GET, POST, DELETE, PUT};
    UserData userData;
    QNetworkAccessManager *netMan;




    QByteArray createTicket(QByteArray array);
    QByteArray getTickets();
    QByteArray getServerData(QNetworkRequest req, TYPE t, QByteArray postData = "");

};

#endif // NETWORK_H
