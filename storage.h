#ifndef STORAGE_H
#define STORAGE_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QMap>

#include "network.h"



class Storage : public Network
{
public:
    Storage(QObject *parent = nullptr);
    bool updateStorage(void);

protected:
    QByteArray getTickets(int count);
//    QByteArray getTickets();

private:
//    enum TYPE {GET, POST, DELETE, PUT};
//    QByteArray getServerData(QNetworkRequest req, TYPE t, QByteArray postData = "");


};

#endif // STORAGE_H
