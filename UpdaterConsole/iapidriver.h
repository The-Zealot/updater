#ifndef IAPIDRIVER_H
#define IAPIDRIVER_H

#include <QObject>

class IApiDriver : public QObject
{
    Q_OBJECT
public:
    explicit IApiDriver(QObject *parent = nullptr);

    virtual void checkUpdate()      = 0;
    virtual void updateDownload()   = 0;
};

#endif // IAPIDRIVER_H
