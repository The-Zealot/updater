#ifndef IAPIDRIVER_H
#define IAPIDRIVER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

enum class ApiDriverRequest
{
    GetDownloadUrl  = 0,
    CheckUpdate     = 1,
    DownloadFile    = 2,
    ReadFile        = 3,
};

class IApiDriver : public QObject
{
    Q_OBJECT
public:
    explicit IApiDriver(QObject *parent = nullptr);

    virtual void checkUpdate(const QString &public_key)                                                      = 0;
    virtual void downloadUpdate()                                                   = 0;
    virtual void tryDownloadFile(const QString &public_key, const QString &path)    = 0;

signals:
    void onReplyFinished();
    void onDownloaded();

protected slots:
    void replyFinished(QNetworkReply* reply);

protected:
    QNetworkAccessManager* _manager;
    QByteArray _responseData;
    ApiDriverRequest _requestType;
};

#endif // IAPIDRIVER_H
