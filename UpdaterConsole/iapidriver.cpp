#include "iapidriver.h"

IApiDriver::IApiDriver(QObject *parent) : QObject(parent)
{
    _manager = new QNetworkAccessManager(this);

    connect(_manager, &QNetworkAccessManager::finished, this, &IApiDriver::replyFinished);

    _requestType = ApiDriverRequest::GetDownloadUrl;
}

void IApiDriver::replyFinished(QNetworkReply *reply)
{
    _responseData = reply->readAll();

    emit onReplyFinished();
}
