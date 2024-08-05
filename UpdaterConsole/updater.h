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

    bool saveToFile(const QString &fileName);

    QByteArray getHexHash(QString fileName);

    void getAllFiles(QString currentDir);

    bool setRepository(const QString &repository);
    bool checkUpdate();
    void update();
    void download(const QString &fileName);

public slots:
    void compareIni();
    void downloadNext();

signals:
    void onIniCompared();
    void onComplited();

private:
    QString _currentDir;
    QStringList _fileList;
    IApiDriver* _driver;
    ApiDriverType _driverType;

    int _currentUpdatingIndex;
    QStringList _updatingList;
    QString _publicKey;
};


#endif // UPDATER_H
