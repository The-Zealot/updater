#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>

#include "updater.h"
#include "yandexdiskdownloader.h"

#define VERSION_MAJOR   0
#define VERSION_MINOR   1

#define ERROR_CODE_NO_ERROR             0
#define ERROR_CODE_NO_CONFIG            1
#define ERROR_CODE_INVALID_ARGUMENTS    2

#define COMMAND_FILE_LIST   "/files"
#define COMMAND_MAKE_INI    "/makeini"
#define COMMAND_UPDATE      "/update"
#define COMMAND_HELP        "/help"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    if (argc > 1)
    {
        Updater* updater = new Updater(ApiDriverType::YandexDisk);
        QObject::connect(updater, &Updater::onComplited, [](){
            exit(ERROR_CODE_NO_ERROR);
        });
        QJsonObject configJson;

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

        updater->setRepository(configJson["repository"].toString());

        if (a.arguments().at(1) == COMMAND_MAKE_INI)
        {
            updater->getFileList();
            updater->saveToFile();

            qDebug() << "includes.ini was updated";
            qDebug() << updater->getFileList().size() << "strings was changed";

            return ERROR_CODE_NO_ERROR;
        }
        if (a.arguments().at(1) == COMMAND_FILE_LIST)
        {
            qDebug() << "List of all entry files:";
            QStringList list = updater->getFileList();
            for (auto iter : list)
            {
                qDebug() << iter;
            }

            return ERROR_CODE_NO_ERROR;
        }
        if (a.arguments().at(1) == COMMAND_UPDATE)
        {
            if (a.arguments().size() < 3)
            {
                return ERROR_CODE_INVALID_ARGUMENTS;
            }

            if (a.arguments().at(2) == "/?")
            {
                qDebug() << "Send GET request to the repository to update";

                qDebug() << "\n /update /d \n /update /c \n";

                qDebug() << "Argument /? is used to display the help";
                qDebug() << "Argument /d is used to download files from repository";
                qDebug() << "Argument /c is used to check files for updates";

                return ERROR_CODE_NO_ERROR;
            }
            if (a.arguments().at(2) == "/c")
            {
                qDebug() << "Current remote repository:" << configJson["repository"];
                qDebug() << "Connecting...";

                updater->checkUpdate();
            }
            if (a.arguments().at(2) == "/d")
            {
                updater->update();
            }
        }
        if (a.arguments().at(1) == COMMAND_HELP or a.arguments().at(1) == "/?")
        {
            if (a.arguments().size() < 3)
            {
                qDebug() << "use  /files    to show all files in this directory";
                qDebug() << "use  /makeini  to create an ini file for correctly updates";
                qDebug() << "use  /update   for total updating current directory";
                qDebug() << "use  /help     to show help information";
                qDebug() << "use  /help /v  to show current application version";

                return ERROR_CODE_NO_ERROR;
            }
            else if (a.arguments().at(2) == "/v")
            {
                qDebug() << "current app version:" << QString::number(VERSION_MAJOR) + "." + QString::number(VERSION_MINOR);

                return ERROR_CODE_NO_ERROR;
            }
        }
    }

    return a.exec();
}
