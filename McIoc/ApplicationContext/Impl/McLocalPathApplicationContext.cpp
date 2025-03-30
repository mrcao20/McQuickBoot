/*
 * Copyright (c) 2021 mrcao20/mrcao20@163.com
 * McQuickBoot is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
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
