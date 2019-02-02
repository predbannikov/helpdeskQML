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

    }
    return true;
}

QByteArray Storage::getTickets(int count)
{
    QString tickets = userData.path + "/tickets/";
    QStringList listDir = QDir(tickets).entryList(QDir::Files);


    QFile file;
    QJsonObject obj;
    for(int i = 0; i < count || i < listDir.size(); i++) {
        QString fileName = tickets + listDir[i];
//        qDebug() << fileName;
        file.setFileName(fileName);
        if(!file.open(QFile::ReadOnly | QFile::Text)) {
            qDebug() << "not open file <ticket> to read and send to qml";
            return QByteArray();
        }
        QJsonParseError jsonParseError;
        QJsonDocument jdoc = QJsonDocument::fromJson(file.readAll(), &jsonParseError);
        obj[listDir[i]] = jdoc.object();
        file.close();
    }
    return QJsonDocument(obj).toJson();

}
