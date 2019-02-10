#include "network.h"

Network::Network(QObject *parent) : QObject(parent)
{
    netMan = new QNetworkAccessManager(this);
}

QByteArray Network::requestTypeJson(QByteArray &packJson, QString url, TYPE type)
{
    QByteArray packLogin = "Basic " + QByteArray(QString(userData.email + ":" + userData.pass).toUtf8()).toBase64();
    QUrlQuery postData;
    QString query = postData.toString(QUrl::FullyEncoded);
    QUrl urlquery(url + query);
    QNetworkRequest request(urlquery);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    request.setRawHeader("Authorization",  packLogin);
    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray array = getServerData(request, type, packJson);
    return array;
}

QByteArray Network::updateTicket(QByteArray &packJson, int id)
{
    QString url = QString("https://designmobile.helpdeskeddy.com/api/v2/tickets/%1/").arg(QString::number(id));
    return requestTypeJson(packJson, url, PUT);
}


QByteArray Network::getTickets()
{
    QByteArray packLogin = "Basic " + QByteArray(QString(userData.email + ":" + userData.pass).toUtf8()).toBase64();
    QString url("https://designmobile.helpdeskeddy.com/api/v2/tickets?");

    QUrlQuery postData;
    postData.addQueryItem("from_date_updated", "2017-12-31 00:00:00");
    postData.addQueryItem("to_date_updated" , QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    postData.addQueryItem("order_by", "date_updated");
    QString query = postData.toString(QUrl::FullyEncoded);
    QUrl urlquery(url + query);
    QNetworkRequest request(urlquery);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    request.setRawHeader("Authorization",  packLogin);
    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QByteArray array = getServerData(request, GET);
    return array;
}

QByteArray Network::getTicketsRange(QString date_start, QString date_end, bool sort_asc)
{
    QByteArray packLogin = "Basic " + QByteArray(QString(userData.email + ":" + userData.pass).toUtf8()).toBase64();
    QString url("https://designmobile.helpdeskeddy.com/api/v2/tickets?");

    QUrlQuery postData;
    postData.addQueryItem("from_date_created", QDateTime::currentDateTime().toString(date_start));
    postData.addQueryItem("to_date_created" , QDateTime::currentDateTime().toString(date_end));
    if(sort_asc)
        postData.addQueryItem("order_by", "asc");
    QString query = postData.toString(QUrl::FullyEncoded);
    QUrl urlquery(url + query);


    QNetworkRequest request(urlquery);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    request.setRawHeader("Authorization",  packLogin);
    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QByteArray array = getServerData(request, GET);
    return array;
}

QByteArray Network::getServerData(QNetworkRequest req, Network::TYPE t, QByteArray postData)
{
    QEventLoop loop;
    QNetworkReply *reply;
    switch(t)
    {
    case GET:
        reply = netMan->get(req);
        break;
    case POST:
        reply = netMan->post(req, postData);
        break;
    case DELETE:
        reply = netMan->deleteResource(req);
        break;
    case PUT:
        reply = netMan->put(req, postData);
        break;
    }
    if (reply == nullptr) {
        qWarning("Reply is NULL");
        delete reply;
        return QByteArray();
    }
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if (reply->error() != QNetworkReply::NoError) {
        qCritical("%s", qPrintable(QString("[%1] %2").arg(reply->error()).arg(reply->errorString())));
        QByteArray data = reply->readAll();
        qDebug() << data;
        delete reply;
        return QByteArray("Error getServerData");
    }
    QByteArray data = reply->readAll();
    qDebug() << data;
    reply->deleteLater();
    delete reply;
    return data;
}

QByteArray Network::createTicket(QByteArray array)
{
    QJsonParseError jsonParseError;
    QJsonDocument jdoc = QJsonDocument::fromJson(array, &jsonParseError);
    if(jsonParseError.error != QJsonParseError::NoError)
    {
        qDebug() << "createTicket::jsonParseError.error" << jsonParseError.errorString();
        return QByteArray();
    }
    QJsonObject jobj = jdoc.object();
    QString email = jobj["email"].toString();
    QString description = jobj["description"].toString();
    QString title = jobj["title"].toString();

    QUrlQuery postData;
    postData.addQueryItem("title", title);
    postData.addQueryItem("description" , QString("<p>%1 %2</p>").arg(description).arg(email));
    QByteArray postDataEncoded = postData.toString(QUrl::FullyEncoded).toUtf8();


    QByteArray packLogin = "Basic " + QByteArray(QString(userData.email + ":" + userData.pass).toUtf8()).toBase64();
    QUrl url("https://designmobile.helpdeskeddy.com/api/v2/tickets/");
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    request.setRawHeader("Authorization",  packLogin);
    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    array = getServerData(request, POST, postDataEncoded);
    qDebug() << array;
    return array;
}
