#ifndef STORAGE_H
#define STORAGE_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QMap>
#include <QThread>

#include "network.h"


struct DataStor {
    QString endData;
};


class Storage : public Network
{
public:
    Storage(QObject *parent = nullptr);
    bool updateStorage(void);

protected:
    DataStor dataStor;
    QByteArray getTickets(int startIndex, int endIndex);
    void saveTickets(QByteArray array, MessageProcess &msgProc);
    void launchDownloadTicket();
    int counter = 0;
    QString thatMore(QString date1, QString date2);

    virtual void isDataTicket(QByteArray jarray) = 0;

    QString loadEndData();
    void saveEndData(QString endData);
private:
};

#endif // STORAGE_H
