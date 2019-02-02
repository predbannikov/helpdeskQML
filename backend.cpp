#include "backend.h"

BackEnd::BackEnd(QObject *parent) : Storage(parent)
{

    QCoreApplication::setApplicationName("Helpdesk");
    pathApp = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    qDebug() << pathApp;

    if(!QDir(pathApp).exists())
    {
        if(QDir().mkdir(pathApp)) {
            qDebug() << "success create folder";
        } else {
            qDebug() << "folder not created";
            return;
        }
    }

    connect(this, &BackEnd::updateTicketsList, this, &Storage::updateStorage);


}
BackEnd::~BackEnd()
{
    delete netMan;
}

void BackEnd::initUserData(QByteArray array)
{
    QJsonParseError parseError;
    QJsonDocument jdoc = QJsonDocument::fromJson(array, &parseError);
    if(parseError.error != QJsonParseError::NoError)
    {
        qDebug() << "Error initUserData: " << parseError.errorString();
        return ;
    }
    QJsonObject jobj = jdoc.object();
    userData.email = jobj["email"].toString();
    userData.pass = jobj["password"].toString();
    userData.path = jobj["path"].toString();
    userData.active = true;
}

void BackEnd::printUserData()
{
    qDebug() << userData.email;
    qDebug() << userData.pass;
    qDebug() << userData.path;
    qDebug() << userData.active;

}

void BackEnd::getDataStor(QByteArray array)
{
    QJsonParseError jsonParseError;
    QJsonDocument jdoc = QJsonDocument::fromJson(array, &jsonParseError);
    QJsonObject obj = jdoc.object();
    QByteArray respArray;
    if(obj.contains("tickets")) {
        int count = obj["count"].toInt();


        respArray = getTickets(count);
    }
    if(respArray.isEmpty()) {
        qDebug() << "getDataStor: respArray.isEmpty";
        return;
    }

    emit sendField(respArray);
}

bool BackEnd::readUserDataActive(QString &query)
{
    qDebug() << "path =" << query;
    QFile fileData(query + "/userData.json");
    if(!fileData.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "not open to read file userData.json of file " <<  query;
        return false;
    }
    QByteArray data = fileData.readAll();
    fileData.close();
    QJsonParseError parsError;
    QJsonDocument jdoc = QJsonDocument::fromJson(data, &parsError);
    if(parsError.error != QJsonParseError::NoError)
    {
        qDebug() << "error" <<  data;
        return false;
    }
    QJsonObject jobj = jdoc.object();
    if(jobj["active"].toBool()) {
        query = jdoc.toJson();
        return true;
    }
    return false;
}

bool BackEnd::saveUserData()
{
    qint64 utc = QDateTime::currentSecsSinceEpoch();
    QString folder = QString::number(utc);
    if(!QDir(pathApp).mkdir(folder)) {
        qDebug() << "not create dirrectory for save userData";
        return false;
    }

    userData.path = pathApp + "/" + folder;
    QString tickets = userData.path + "/tickets/";



    if(!QDir(tickets).exists()) {
        if(QDir().mkdir(tickets))
        {
            qDebug() << "success create folder <tickets> ";
        } else {
            qDebug() << "folder <tickets> not created";
            return false;
        }
    }

    QFile fileData(userData.path + "/userData.json");
    if(!fileData.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "not open to write file userData.json of file " <<  fileData.fileName();
        return false;
    }
    fileData.write(userDataToJson().toJson());
    fileData.close();
    return true;
}

bool BackEnd::saveActiveUserData()
{
    QStringList listDir = QDir(pathApp).entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    if(listDir.isEmpty()) {
        qDebug() << "user not found";
        return false;
    }
    foreach (QString subdir, listDir) {
        QFile fileData(pathApp + "/" + subdir + "/userData.json");
        if(!fileData.open(QFile::ReadOnly | QFile::Text)) {
            qDebug() << "not open to read file userData.json of file ";
            return false;
        }
        QByteArray data = fileData.readAll();
        fileData.close();
        QJsonParseError parsError;
        QJsonDocument jdoc = QJsonDocument::fromJson(data, &parsError);
        if(parsError.error != QJsonParseError::NoError)
        {
            qDebug() << "error jsonParseError" <<  data;
            return false;
        }

        if(jdoc.object()["email"].toString() == userData.email) {
            QFile fileData(userData.path +  "/userData.json");
            if(!fileData.open(QFile::WriteOnly | QFile::Text)) {
                qDebug() << "not open to write file userData.json of file " <<  fileData.fileName();
                return false;
            }
            fileData.write(userDataToJson().toJson());
            fileData.close();
            return true;
        }
    }
    return false;
}

void BackEnd::clearUserData()
{
    userData.active = false;
    userData.email.clear();
    userData.pass.clear();
    userData.path.clear();
}

bool BackEnd::findUserData()
{
    QStringList listDir = QDir(pathApp).entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (QString subdir, listDir) {
        QFile fileData(pathApp + "/" + subdir + "/userData.json");
        if(!fileData.open(QFile::ReadOnly | QFile::Text)) {
            qDebug() << "not open to read file userData.json of file ";
            return false;
        }
        QByteArray data = fileData.readAll();
        fileData.close();
        QJsonParseError parsError;
        QJsonDocument jdoc = QJsonDocument::fromJson(data, &parsError);
        if(parsError.error != QJsonParseError::NoError)
        {
            qDebug() << "error jsonParseError" <<  data;
            return false;
        }
        QJsonObject jobj = jdoc.object();
        if(userData.email == jobj["email"].toString()) {
            if(userData.pass == jobj["password"].toString()) {
                userData.path = jobj["path"].toString();
                return true;
            }
        }
    }
    return false;
}

void BackEnd::jsonToUserData(QJsonDocument jdoc)
{
    userData.active = jdoc.object()["active"].toBool();
    userData.email = jdoc.object()["email"].toString();
    userData.pass = jdoc.object()["password"].toString();
    userData.path = jdoc.object()["path"].toString();
}

QJsonDocument BackEnd::userDataToJson()
{
    QJsonObject jobj;
    jobj["email"] = userData.email;
    jobj["password"] = userData.pass;
    jobj["active"] = userData.active;
    jobj["path"] = userData.path;
    return QJsonDocument(jobj);
}

//************************** START ****************************
void BackEnd::findActiveUser()
{
    QStringList listDir = QDir(pathApp).entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (QString subdir, listDir) {
        QString query = pathApp + "/" + subdir;
        if(readUserDataActive(query)) {
            qDebug() << "Active user data found" << pathApp + "/" << subdir;
            login(query.toUtf8());
            break;
        }
    }
}

void BackEnd::login(QByteArray jarray)
{
    qDebug() << "*******LOGIN*******";
    initUserData(jarray);
    if(!findUserData()) {
        // Новый пользователь
        qDebug() << "Login not found, try create account";
        if(!saveUserData())
        {
            qDebug() << "not save userData";
            return;
        } else
            qDebug() << "success save userData";

    } else {
        if(!saveActiveUserData()) {
            qDebug() << "not save userData active state";
            return;
        }
        else
            qDebug() << "success save userData active state";
    }
    emit sendUserData(userDataToJson().toJson());
    return;
}

void BackEnd::logout()
{
    qDebug() << "LOGOUT";
    userData.active = false;
    if(!saveActiveUserData())
        qDebug() << "not save userData active state";
    else
        qDebug() << "success save userData active state";
    clearUserData();
}

void BackEnd::eventTimer()
{
    qDebug() << "time event";
    if(userData.active)
        emit updateTicketsList();
}


void BackEnd::ticketCreate(QByteArray array)
{
    createTicket(array);
}
