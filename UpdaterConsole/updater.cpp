#include "updater.h"

Updater::Updater()
{
    _currentDir = QDir::currentPath();
}

Updater::~Updater()
{

}

QStringList Updater::getFileList()
{
    getAllFiles(_currentDir);

    return _fileList;
}

bool Updater::saveToFile()
{
    QFile file("includes.ini");
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

void Updater::getAllFiles(QString currentDir)
{
    QDir dir(currentDir);
    for (auto & iter : dir.entryInfoList())
    {
        if (iter.isDir())
        {
            if (iter.fileName() != "." and iter.fileName() != "..")
            {
                getAllFiles(iter.filePath());
            }
        }
        else
        {
            QString file = iter.filePath().replace(_currentDir, ".");
            _fileList.append(file);
        }
    }
}
