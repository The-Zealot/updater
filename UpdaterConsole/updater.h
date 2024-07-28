#ifndef UPDATER_H
#define UPDATER_H

#include <exception>
#include <QObject>
#include <QCryptographicHash>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QSettings>

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

    bool setRepository(const QString &repository);
    bool checkUpdate();
    void fullUpdate(QStringList &fileList);

public slots:
    QStringList compareIni();

private:
    QString _currentDir;
    QStringList _fileList;
    IApiDriver* _driver;
    ApiDriverType _driverType;

    QString _publicKey;
};


#endif // UPDATER_H
