#ifndef UPDATER_H
#define UPDATER_H

#include <exception>
#include <QObject>
#include <QCryptographicHash>
#include <QDebug>
#include <QFile>
#include <QDir>

#include "yandexdiskdriver.h"
#include "googledrivedriver.h"
#include "apidrivertype.h"

class Updater
{
public:
    Updater(ApiDriverType driver);
    ~Updater();

    QStringList getFileList();

    bool saveToFile();

    QByteArray getHexHash(QString fileName);

    void getAllFiles(QString currentDir);

private:
    QString _currentDir;
    QStringList _fileList;
    IApiDriver* _driver;
    ApiDriverType _driverType;
};


#endif // UPDATER_H
