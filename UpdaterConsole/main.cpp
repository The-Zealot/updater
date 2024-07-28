#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>

#include "updater.h"
#include "yandexdiskdownloader.h"

#define CLIENT_ID "00fa0ee81e0a4001af38352ec8c92749"
#define DEBUG_TOKEN "y0_AgAAAAAiydKTAAwPKwAAAAEJivHTAAB26_lvlvxAm7dNpQgp9SqA4l3-2w"
#define DEBUG_TOKEN_V2 "y0_AgAAAAAiydKTAAwPKwAAAAEJivHTAAB26_lvlvxAm7dNpQgp9SqA4l3-2w"


#define ERROR_CODE_NO_CONFIG 1

#define COMMAND_FILE_LIST "/files"
#define COMMAND_MAKE_INI "/makeini"
#define COMMAND_UPDATE "/update"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc > 1)
    {
        Updater* updater = new Updater(ApiDriverType::YandexDisk);
        QJsonObject configJson;
        YandexDiskDownloader* downloader = new YandexDiskDownloader();

        QFile file("./config.json");
        if (file.open(QIODevice::ReadOnly))
        {
            configJson = QJsonDocument::fromJson(file.readAll()).object();
            file.close();
        }
        else
        {
            qDebug() << "Can't read coniguration file";
            return ERROR_CODE_NO_CONFIG;
        }

        if (a.arguments().at(1) == COMMAND_MAKE_INI)
        {
            updater->getFileList();
            updater->saveToFile();

            qDebug() << "includes.ini was updated";
            qDebug() << updater->getFileList().size() << "strings was changed";
        }
        if (a.arguments().at(1) == COMMAND_FILE_LIST)
        {
            qDebug() << "List of all entry files:";
            QStringList list = updater->getFileList();
            for (auto iter : list)
            {
                qDebug() << iter;
            }
        }
        if (a.arguments().at(1) == COMMAND_UPDATE)
        {
            qDebug() << "Current remote repository:" << configJson["repository"];
            qDebug() << "Connecting...";

//            downloader->getFileList();

            updater->checkUpdate();
//            downloader->auth();
        }
    }

    return a.exec();
}
