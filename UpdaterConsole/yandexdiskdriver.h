#ifndef YANDEXDISKDRIVER_H
#define YANDEXDISKDRIVER_H

#include "iapidriver.h"
#include <QObject>

class YandexDiskDriver : public IApiDriver
{
    Q_OBJECT
public:
    explicit YandexDiskDriver(QObject *parent = nullptr);

    void checkUpdate() override;
    void updateDownload() override;
};

#endif // YANDEXDISKDRIVER_H
