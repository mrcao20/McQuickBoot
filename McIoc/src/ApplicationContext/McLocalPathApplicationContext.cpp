/*
 * MIT License
 *
 * Copyright (c) 2021 mrcao20
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
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
