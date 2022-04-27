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
#include "McLocalPathApplicationContext.h"

#include <QDebug>
#include <QFile>

McLocalPathApplicationContext::McLocalPathApplicationContext() noexcept {}

McLocalPathApplicationContext::McLocalPathApplicationContext(const QString &location, const QString &flag) noexcept
    : McLocalPathApplicationContext(QStringList() << location, flag)
{
}

McLocalPathApplicationContext::McLocalPathApplicationContext(const QStringList &locations, const QString &flag) noexcept
{
    QList<QIODevicePtr> devices;
    for (auto &location : locations) {
        auto filePath = Mc::toAbsolutePath(location);
        if (!QFile::exists(filePath)) {
            qCCritical(mcIoc(), "file '%s' not exists", qUtf8Printable(filePath));
            continue;
        }
        auto device = QSharedPointer<QFile>::create(filePath);
        if (!device->open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCCritical(mcIoc()) << "open file '" << filePath << "' failure."
                                << "error code:" << device->error() << "error msg:" << device->errorString();
            continue;
        }
        devices.append(device);
    }
    setDevices(devices, flag);
}
