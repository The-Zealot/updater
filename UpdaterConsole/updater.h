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

class Updater : public QObject
{
    Q_OBJECT
public:
    explicit Updater(ApiDriverType driver, QObject* parent = nullptr);
    ~Updater();

    QStringList getFileList();

    bool saveToFile();

    QByteArray getHexHash(QString fileName);

    void getAllFiles(QString currentDir);

    bool checkUpdate();

private:
    QString _currentDir;
    QStringList _fileList;
    IApiDriver* _driver;
    ApiDriverType _driverType;
};


#endif // UPDATER_H
