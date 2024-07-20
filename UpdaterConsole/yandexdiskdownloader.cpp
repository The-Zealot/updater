#include "yandexdiskdownloader.h"

YandexDiskDownloader::YandexDiskDownloader(QObject *parent) : QObject(parent)
{
    _manager = new QNetworkAccessManager(this);
    connect(_manager, &QNetworkAccessManager::finished, this, &YandexDiskDownloader::replyFinished);
    connect(this, &YandexDiskDownloader::onAuth, this, &YandexDiskDownloader::download);
    connect(this, &YandexDiskDownloader::onGetFileList, this, &YandexDiskDownloader::fileList);

    _status = YandexDiskDownloaderStatus::Listening;
}

YandexDiskDownloader::~YandexDiskDownloader()
{
    delete _manager;
}

void YandexDiskDownloader::auth()
{
    QNetworkRequest request;
    //    request.setUrl(QUrl("https://disk.yandex.ru/client/disk/QtYandexDiskTest"));

    QString host = "cloud-api.yandex.net";
    QString api = "/v1/disk/public/resources/download";
    QString public_key = "https://disk.yandex.ru/d/H9UuULUw5x06oA";
    QString path = "/includes.ini";

    request.setUrl(QUrl("https://" + host + api + "?public_key=" + public_key + "&path=" + path));

    //    request.setRawHeader("Authorization", "OAuth y0_AgAAAAAiydKTAAwPKwAAAAEJivHTAAB26_lvlvxAm7dNpQgp9SqA4l3-2w");

    _status = YandexDiskDownloaderStatus::Auth;
    _manager->get(request);

}

void YandexDiskDownloader::download()
{
    QJsonObject jObject = QJsonDocument::fromJson(_replyData).object();

    _currentFile = jObject["href"].toString();
    qDebug() << _currentFile;

    QNetworkRequest request(_currentFile);
    _status = YandexDiskDownloaderStatus::Updating;
    _manager->get(request);
}

void YandexDiskDownloader::replyFinished(QNetworkReply* reply)
{
    _replyData.clear();
    _replyData = reply->readAll();
    reply->deleteLater();

    YandexDiskDownloaderStatus temp = _status;
    _status = YandexDiskDownloaderStatus::Listening;

    switch (temp)
    {
    case YandexDiskDownloaderStatus::GettingFileList:
        emit onGetFileList();
        break;
    case YandexDiskDownloaderStatus::Listening:
        break;
    case YandexDiskDownloaderStatus::Auth:
        emit onAuth();
        break;
    case YandexDiskDownloaderStatus::CheckUpdate:
        break;
    case YandexDiskDownloaderStatus::Updating:
        qDebug() << "File:";
        qDebug() << reply->readAll();
        break;
    }


}

void YandexDiskDownloader::getFileList()
{
    QNetworkRequest request;

    QString host = "cloud-api.yandex.net";
    QString api = "/v1/disk/public/resources";
    QString public_key = "https://disk.yandex.ru/d/H9UuULUw5x06oA";

    request.setUrl(QUrl("https://" + host + api + "?public_key=" + public_key));

    _status = YandexDiskDownloaderStatus::GettingFileList;
    _manager->get(request);
}

void YandexDiskDownloader::fileList()
{
    QJsonObject jObject = QJsonDocument::fromJson(_replyData).object();
    QJsonArray jArray   = jObject["_embedded"].toObject()["items"].toArray();

    for (auto iter : jArray)
    {
        _fileList.append(iter.toObject()["name"].toString());
    }

    qDebug() << _fileList;
}
