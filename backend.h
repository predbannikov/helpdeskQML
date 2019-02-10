#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QByteArray>
#include <QEventLoop>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDateTime>
#include <QStandardPaths>
#include <QTimer>
#include <QThread>

#include "storage.h"
//#include "network.h"


class BackEnd : public Storage
{
    Q_OBJECT
public:
    explicit BackEnd(QObject *parent = nullptr);
    void findActiveUser();
    virtual ~BackEnd();
private:
    QString pathApp;


    void initUserData(QByteArray array);
    void clearUserData();

    bool findUserData();
    bool readUserDataActive(QString &query);

    bool saveUserData();
    bool saveActiveUserData();


    void jsonToUserData(QJsonDocument jdoc);
    QJsonDocument userDataToJson();

    void printUserData();

signals:
    void sendUserData(QByteArray jsonArray);
    void sendField(QByteArray field);
    void updateTicketsList();

    void sendCustomData(QByteArray jarray);


public slots:
    void isDataTicket(QByteArray jarray);
    void ticketCreate(QByteArray array);
    void login(QByteArray jarray);
    void getDataStor(QByteArray array);
    void logout();
    void eventTimer();

    void updateStatusTicket(QByteArray packJson, int id);
};



#endif // BACKEND_H
