#ifndef GOOGLEDRIVEDRIVER_H
#define GOOGLEDRIVEDRIVER_H

#include "iapidriver.h"
#include <QObject>

class GoogleDriveDriver : public IApiDriver
{
    Q_OBJECT
public:
    explicit GoogleDriveDriver(QObject *parent = nullptr);


//    void downloadUpdate() override;
};

#endif // GOOGLEDRIVEDRIVER_H
