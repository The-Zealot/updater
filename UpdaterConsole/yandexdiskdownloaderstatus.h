#ifndef YANDEXDISKDOWNLOADERSTATUS_H
#define YANDEXDISKDOWNLOADERSTATUS_H

enum class YandexDiskDownloaderStatus
{
    Auth            = 0,
    CheckUpdate     = 1,
    Updating        = 2,
    GettingFileList = 3,
    Listening       = 255,
};

#endif // YANDEXDISKDOWNLOADERSTATUS_H
