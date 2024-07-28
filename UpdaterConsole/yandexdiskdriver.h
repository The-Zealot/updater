#ifndef YANDEXDISKDRIVER_H
#define YANDEXDISKDRIVER_H

#include "iapidriver.h"
#include <QObject>
#include <QFile>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

struct YDFile
{
    QString name;
    QString href;
};

class YandexDiskDriver : public IApiDriver
{
    Q_OBJECT
public:
    explicit YandexDiskDriver(QObject *parent = nullptr);

    void checkUpdate(const QString &public_key) override;
    void downloadUpdate(const QString &public_key, QStringList &updateFileList) override;
    void tryDownloadFile(const QString &public_key, const QString &path) override;

    QByteArray getLastData() override;

private:
    void downloadFile(const QString &url);

    QNetworkRequest buildRequest(const QString &api, const QString &params, ApiDriverRequest requestType);

public slots:
    void process();

signals:
    void onUpdateChecked();
    void onUpdated();
    void onDownloadUrlGot();

private:
    YDFile _file;
    QString _host;
};

#endif // YANDEXDISKDRIVER_H
