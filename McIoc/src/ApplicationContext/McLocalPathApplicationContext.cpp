#include "McIoc/ApplicationContext/impl/McLocalPathApplicationContext.h"

#include <QFile>
#include <QDebug>

McLocalPathApplicationContext::McLocalPathApplicationContext(QObject *parent)
    : McXmlApplicationContext(parent)
{
}

McLocalPathApplicationContext::McLocalPathApplicationContext(const QString &location,
                                                             const QString &flag,
                                                             QObject *parent)
    : McLocalPathApplicationContext(QStringList() << location, flag, parent)
{
}

McLocalPathApplicationContext::McLocalPathApplicationContext(const QStringList &locations,
                                                             const QString &flag,
                                                             QObject *parent)
    : McLocalPathApplicationContext(parent)
{
    QList<QIODevicePtr> devices;
    for(auto location : locations) {
        if(!QFile::exists(location)) {
            qCritical() << "file '" << location << "' not exists";
            continue;
        }
        auto device = new QFile(location);
        if (!device->open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCritical() << "open file '" << location << "' failure."
                        << "error code:" << device->error()
                        << "error msg:" << device->errorString();
            continue;
        }
        devices.append(QIODevicePtr(device));
    }
    setDevices(devices, flag);
}
