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
        auto filePath = Mc::toAbsolutePath(location);
        if (!QFile::exists(filePath)) {
            qCritical() << "file '" << filePath << "' not exists";
            continue;
        }
        auto device = new QFile(filePath);
        if (!device->open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCritical() << "open file '" << filePath << "' failure."
                        << "error code:" << device->error()
                        << "error msg:" << device->errorString();
            device->deleteLater();
            continue;
        }
        devices.append(QIODevicePtr(device));
    }
    setDevices(devices, flag);
}
