#include "include/ApplicationContext/impl/McLocalPathApplicationContext.h"

#include <QFile>
#include <QDebug>

McLocalPathApplicationContext::McLocalPathApplicationContext(QObject *parent)
    : McXmlApplicationContext(parent)
{
}

McLocalPathApplicationContext::McLocalPathApplicationContext(
        const QString &location, QObject *parent)
    : McLocalPathApplicationContext(QStringList() << location, parent)
{
}

McLocalPathApplicationContext::McLocalPathApplicationContext(
        const QStringList &locations, QObject *parent)
    : McLocalPathApplicationContext(parent)
{
    QList<QIODevicePtr> devices;
    for(auto location : locations) {
        if(!QFile::exists(location)) {
            qCritical() << "file '" << location << "' not exists";
            continue;
        }
        auto device = new QFile(location);
        if (!device->open(QIODevice::ReadOnly)) {
            qCritical() << "open file '" << location << "' failure."
                        << "error code:" << device->error()
                        << "error msg:" << device->errorString();
            continue;
        }
        devices.append(QIODevicePtr(device));
    }
    setDevices(devices);
}
