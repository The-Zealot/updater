#include "yandexdiskdriver.h"

YandexDiskDriver::YandexDiskDriver(QObject *parent) : IApiDriver(parent)
{
    connect(this, &YandexDiskDriver::onReplyFinished, this, &YandexDiskDriver::process);

    _host = "cloud-api.yandex.net";
}

void YandexDiskDriver::checkUpdate(const QString &public_key)
{
    qDebug() << "Checking updates from YandexDisk...";
//    request.setUrl(QUrl("https://" + _host + api + "?public_key=" + public_key + "&path=/include.ini"));

    QString params = "?public_key=" + public_key + "&path=/includes.ini";
    _manager->get(buildRequest("/v1/disk/public/resources/download", params, ApiDriverRequest::CheckUpdate));
}

void YandexDiskDriver::downloadUpdate()
{

}

void YandexDiskDriver::tryDownloadFile(const QString &public_key, const QString &path)
{
    qDebug() << "Trying download from YandexDisk...";
    _file.name = path;
//    request.setUrl(QUrl("https://" + _host + api + "?public_key=" + public_key + "&path=/" + path));


    QString params = "?public_key=" + public_key + "&path=/" + path;
    _manager->get(buildRequest("/v1/disk/public/resources/download", params, ApiDriverRequest::GetDownloadUrl));
}

void YandexDiskDriver::downloadFile(const QString &url)
{
    qDebug() << "Download...";

    QNetworkRequest request(url);
    _requestType == ApiDriverRequest::GetDownloadUrl ? _requestType = ApiDriverRequest::DownloadFile : _requestType = ApiDriverRequest::ReadFile;
    _manager->get(request);
}

QNetworkRequest YandexDiskDriver::buildRequest(const QString &api, const QString &params, ApiDriverRequest requestType)
{
    QNetworkRequest request;
    request.setUrl(QUrl("https://" + _host + api + params));
    _requestType = requestType;

    return request;
}

void YandexDiskDriver::process()
{
    QJsonObject jObject;
    QFile file(_file.name);

    switch (_requestType)
    {
    case ApiDriverRequest::GetDownloadUrl: case ApiDriverRequest::CheckUpdate:
        jObject = QJsonDocument::fromJson(_responseData.data()).object();

        if (!jObject["templated"].toBool())
        {
            _file.href = jObject["href"].toString();
            downloadFile(_file.href);
        }
        else
        {
            qDebug() << jObject["error"].toString();
        }

        break;
    case ApiDriverRequest::DownloadFile:
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(_responseData);
            file.close();
        }
        else
        {
            qDebug() << "Failed download";
        }

        emit onDownloaded();

        break;
    case ApiDriverRequest::ReadFile:
        qDebug () << _responseData.data();
        break;
    }
}
