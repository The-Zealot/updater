#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QCryptographicHash>
#include <QDebug>
#include <QFile>
#include <QDir>

#include <iapidriver.h>

class Updater
{
public:
    Updater();
    ~Updater();

    QStringList getFileList();

    bool saveToFile();

    QByteArray getHexHash(QString fileName);

    void getAllFiles(QString currentDir);

private:
    QString _currentDir;
    QStringList _fileList;
    IApiDriver* _driver;
};


#endif // UPDATER_H
