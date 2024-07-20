#ifndef YANDEXDISKDOWNLOADER_H
#define YANDEXDISKDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "yandexdiskdownloaderstatus.h"

class YandexDiskDownloader : public QObject
{
    Q_OBJECT
public:
    explicit YandexDiskDownloader(QObject *parent = nullptr);
    ~YandexDiskDownloader();

    void auth();

public slots:
    void download();
    void replyFinished(QNetworkReply* reply);
    void getFileList();

    void fileList();

signals:
    void onAuth();
    void onGetFileList();

private:
    QNetworkAccessManager* _manager;
    YandexDiskDownloaderStatus _status;
    QString _currentFile;
    QStringList _fileList;
    QByteArray _replyData;
};

#endif // YANDEXDISKDOWNLOADER_H
