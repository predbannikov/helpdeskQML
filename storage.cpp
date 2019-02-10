#include "storage.h"

Storage::Storage(QObject *parent) : Network(parent)
{

}

bool Storage::updateStorage()
{
    QByteArray array = Network::getTickets();
    if(array.isEmpty())
        qDebug() << "array getTicket isEmpty";

    QJsonParseError jsonParseError;
    QJsonDocument jdoc = QJsonDocument::fromJson(array, &jsonParseError);
    QString tickets = userData.path + "/tickets/";


    QStringList listDir = QDir(tickets).entryList(QDir::Files);
    QMap <QString, qint64> map;
    for(int i = 0; i < listDir.size(); i++) {
        QStringList split = listDir[i].split('_');
        map.insert(split[0], split[1].toInt());
    }

    QJsonObject obj = jdoc.object();
    QJsonObject obj2 = obj["data"].toObject();

    QJsonArray customArr;

    QFile file;
    foreach (QString key, obj2.keys()) {
        QDateTime dt = QDateTime::fromString(obj2[key].toObject()["date_updated"].toString(), "yyyy-MM-dd hh:mm:ss");
        QString fileName = tickets + key + "_";
        if(map.contains(key)) {
            if(map[key] >= dt.toSecsSinceEpoch())
                continue;
            QFile rm_file(fileName + QString::number(map[key]));
            if(!rm_file.remove()) {
                qDebug() << "file not removed";
                return false;
            }
        }

        fileName.append(QString::number(dt.toSecsSinceEpoch()));

        file.setFileName(fileName);
        if(!file.open(QFile::WriteOnly | QFile::Text)) {
            qDebug() << "ticket not open to write file";
            return false;
        }

        file.write(QJsonDocument(obj2[key].toObject()).toJson());
        file.close();
        customArr.append(obj2[key].toObject());
    }                  // Если при обновлении были изменения, на отправку их в qml
    if(!customArr.isEmpty()) {
        isDataTicket(QJsonDocument(customArr).toJson());
        qDebug() << "sendCustom";
    }
    return true;
}

QByteArray Storage::getTickets(int startIndex, int endIndex)    // qml запрашивает тикеты отсюда по индексам
{
    QString tickets = userData.path + "/tickets/";
    QStringList listFiles = QDir(tickets).entryList(QDir::Files, QDir::Time);


    QFile file;
    QJsonArray jarr;
    for(int i = startIndex; i < endIndex && i < listFiles.size(); i++) {
        QString fileName = tickets + listFiles[i];
        file.setFileName(fileName);
        if(!file.open(QFile::ReadOnly | QFile::Text)) {
            qDebug() << "not open file <ticket> to read and send to qml";
            return QByteArray();
        }
        QJsonParseError jsonParseError;
        QJsonDocument jdoc = QJsonDocument::fromJson(file.readAll(), &jsonParseError);
        jarr.append(jdoc.object());
        file.close();
    }
    return QJsonDocument(jarr).toJson();

}

void Storage::saveTickets(QByteArray array, MessageProcess &msgProc)
{
    QJsonDocument jdoc = QJsonDocument::fromJson(array, &jError);
    QString tickets = userData.path + "/tickets/";

    QJsonObject obj = jdoc.object();
    QJsonObject obj2 = obj["data"].toObject();

    QFile file;
    foreach (QString key, obj2.keys()) {
        QDateTime dt = QDateTime::fromString(obj2[key].toObject()["date_updated"].toString(), "yyyy-MM-dd hh:mm:ss");
        QString fileName = tickets + key + "_";
        fileName.append(QString::number(dt.toSecsSinceEpoch()));
        file.setFileName(fileName);
        if(!file.open(QFile::WriteOnly | QFile::Text)) {
            qDebug() << "LOADER: ticket not open to write file";
            msgProc.check = false;
            return;
        }

        file.write(QJsonDocument(obj2[key].toObject()).toJson());
        file.close();
        msgProc.msg = thatMore(obj2[key].toObject()["date_created"].toString(), msgProc.msg);
    }
}
                                        // Запускать после логина тестовая
                                        // начать процесс скачки всех тикетов
void Storage::launchDownloadTicket()
{
    QString startTime = loadEndData();
    QString endTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    bool sort_asc = true;
    if(startTime.isEmpty()) {
        saveEndData(QDateTime::fromSecsSinceEpoch(0).toString("yyyy-MM-dd hh:mm:ss"));
        sort_asc = true;
        startTime = loadEndData();
    }
                    // Загружаем следующую (+1 по таймстампу) порцию тикетов
    QByteArray array = getTicketsRange(
                QDateTime::fromSecsSinceEpoch(QDateTime::fromString(startTime, "yyyy-MM-dd hh:mm:ss").toSecsSinceEpoch() + 1).toString("yyyy-MM-dd hh:mm:ss"),
                endTime, sort_asc);

    if(array.isEmpty()) {
        qDebug() << "data load is empty";
        return;
    }

    saveTickets(array, msgProc);
    if(!msgProc.check) {
        qDebug() << "LOADER: not save tickets";
        return;
    }
    saveEndData(msgProc.msg);

    if(QJsonDocument::fromJson(array).object()["pagination"].toObject()["total"].toString().toInt() == 0) {
        qDebug() << "Stop LOADER";
        return ;
    }
    counter++;
    QThread::msleep(1000);
    launchDownloadTicket();

}

QString Storage::thatMore(QString date1, QString date2)
{
    if(date1.isEmpty())
        return date2;
    if(date2.isEmpty())
        return date1;
    if(QDateTime::fromString(date1, "yyyy-MM-dd hh:mm:ss").toSecsSinceEpoch() > QDateTime::fromString(date2, "yyyy-MM-dd hh:mm:ss").toSecsSinceEpoch())
        return date1;
    else
        return date2;
}
                                            // Получить 'date_created' последнего скаченного файла
QString Storage::loadEndData()
{
    QFile file(userData.path + "/dataStor.json");
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "no read file dataStor.json";
        return QString();
    }
    QJsonParseError parsError;
    QJsonDocument jdoc = QJsonDocument::fromJson(file.readAll(), &parsError);
    file.close();
    if(parsError.error != QJsonParseError::NoError) {
        qDebug() << "loadDataStor::parsError" << parsError.errorString();
        return QString();
    }
    dataStor.endData = jdoc.object()["endData"].toString();
    return dataStor.endData;
}

void Storage::saveEndData(QString endData)
{
    if(endData.isEmpty())
        return;
    QFile file(userData.path + "/dataStor.json");
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "no write file dataStor.json";
        return ;
    }
    QJsonObject jobj{{"endData", endData}};
    file.write(QJsonDocument(jobj).toJson());
    file.close();
}

