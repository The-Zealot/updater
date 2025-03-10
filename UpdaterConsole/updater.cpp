#include "updater.h"

Updater::Updater(ApiDriverType driver, QObject* parent) : QObject(parent)
{
    switch (driver)
    {
    case ApiDriverType::YandexDisk:
        _driver = new YandexDiskDriver();
        break;
    case ApiDriverType::GoogleDrive:
//        _driver = new GoogleDriveDriver();
        break;
    default:
        throw std::runtime_error("Invalid driver");
    }

    connect(this, &Updater::onIniCompared, this, &Updater::downloadNext);
    connect(_driver, &IApiDriver::onDownloaded, this, &Updater::downloadNext);
    connect(_driver, &IApiDriver::onFileRead, this, &Updater::compareIni);
    connect(_driver, &IApiDriver::onDownloaded, [this](){
        qDebug() << "Update is avilable";
    });

    _currentUpdatingIndex = -1;

    _currentDir = QDir::currentPath();

}

Updater::~Updater()
{
    delete _driver;
}

QStringList Updater::getFileList()
{
    _fileList.clear();

    createFileList(_currentDir);

    return _fileList;
}

bool Updater::saveToFile(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        for (auto & iter : _fileList)
        {
            QByteArray data = iter.toUtf8();
            file.write(data);
            file.write("=");
            file.write(getHexHash(iter));
            file.write("\n");
        }

        file.close();
        return true;
    }
    else
    {
        return false;
    }
}

QByteArray Updater::getHexHash(QString fileName)
{
    fileName = fileName.replace("./", _currentDir + "/");
    QByteArray data;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        data = file.readAll();
        file.close();
    }

    return QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex();
}

void Updater::createFileList(QString currentDir)
{
    QDir dir(currentDir);
    for (auto & iter : dir.entryInfoList())
    {
        if (iter.isDir())
        {
            if (iter.fileName() != "." and iter.fileName() != "..")
            {
                createFileList(iter.filePath());
            }
        }
        else
        {
            QString file = iter.filePath().replace(_currentDir, ".");
            _fileList.append(file);
        }
    }
}

void Updater::compareIni()
{
    _updatingList.clear();
    QFile file("./includes.ini");
    if (file.open(QIODevice::ReadOnly))
    {
        QString tempReadingData     = file.readAll();
        QStringList localIniList    = tempReadingData.split("\n");
        file.close();

        tempReadingData             = _driver->getLastData();
        tempReadingData             = tempReadingData.remove(tempReadingData.size() - 1, 1);
        QStringList remoteIniList   = tempReadingData.split("\n");

        for (auto & iter : remoteIniList)
        {
            if (!localIniList.contains(iter))
            {
                _updatingList.append(iter.replace("./", "").split("=").at(0));
            }
        }
    }
    else
    {
        qDebug() << "Failed when making update file list";
    }

    qDebug() << _updatingList;
    emit onIniCompared();
}

void Updater::downloadNext()
{
    if (_currentUpdatingIndex < 0)
    {
        onComplited();
        return;
    }

    if (_currentUpdatingIndex >= _updatingList.size())
    {
        qDebug() << "Download is complite";
        _currentUpdatingIndex = -1;
        emit onComplited();
        return;
    }

    qDebug() << "Current downloading file: " + _updatingList.at(_currentUpdatingIndex);
    _driver->tryDownloadFile(_publicKey, _updatingList.at(_currentUpdatingIndex));

    _currentUpdatingIndex++;
}

bool Updater::setRepository(const QString &repository)
{
    _publicKey = repository;
}

bool Updater::checkUpdate()
{   
//    _driver->tryDownloadFile("https://disk.yandex.ru/d/H9UuULUw5x06oA", "includes.ini");
    if (!_publicKey.isEmpty())
    {
        _driver->checkUpdate(_publicKey);
        return true;
    }
    else
    {
        qDebug() << "Repository url is empty";
        return false;
    }
}

void Updater::update()
{
    _currentUpdatingIndex = 0;
    _driver->checkUpdate(_publicKey);
}

void Updater::download(const QString &fileName)
{
    _driver->tryDownloadFile(_publicKey, fileName);
}
