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
#include "McIocGlobal.h"

#include <QHash>
#include <QMetaClassInfo>

#include "ApplicationContext/Impl/McAnnotationApplicationContext.h"

Q_LOGGING_CATEGORY(mcIoc, "mc.ioc")

namespace McPrivate {
QString getBeanName(const QMetaObject *metaObj) noexcept
{
    QString beanName;
    auto beanNameIndex = metaObj->indexOfClassInfo(MC_BEANNAME_TAG);
    auto func = [&beanName, metaObj]() {
        beanName = metaObj->className();
        auto str = QLatin1String("::");
        auto index = beanName.lastIndexOf(str);
        if (index != -1) {
            beanName = beanName.mid(index + str.size());
        }
        Q_ASSERT(!beanName.isEmpty());
        auto firstChar = beanName.at(0);
        firstChar = firstChar.toLower();
        beanName.remove(0, 1);
        beanName = firstChar + beanName;
    };
    if (beanNameIndex == -1) {
        func();
    } else {
        auto beanNameClassInfo = metaObj->classInfo(beanNameIndex);
        beanName = beanNameClassInfo.value();
        if (beanName.isEmpty()) {
            func();
        }
    }
    return beanName;
}
} // namespace McPrivate

namespace Mc {
bool isContainedTag(const QByteArray &tags, const QByteArray &tag) noexcept
{
    auto tagList = tags.split(' ');
    for (auto &t : qAsConst(tagList)) {
        if (t == tag) {
            return true;
        }
    }
    return false;
}

QObject *getObject(IMcApplicationContext *appCtx, const QString &beanName) noexcept
{
    if (!appCtx->isContained(beanName)) {
        qCWarning(mcIoc()) << "not exists bean:" << beanName;
        return nullptr;
    }
    if (appCtx->isPointer(beanName)) {
        return appCtx->getBean<QObject *>(beanName);
    } else {
        return appCtx->getBean<QObjectPtr>(beanName).data();
    }
}

namespace Ioc {
void connect(const QString &beanName, const QString &sender, const QString &signal, const QString &receiver,
    const QString &slot, Qt::ConnectionType type) noexcept
{
    McAnnotationApplicationContext::addConnect(beanName, sender, signal, receiver, slot, type);
}

void connect(const QMetaObject *metaObj, const QString &sender, const QString &signal, const QString &receiver,
    const QString &slot, Qt::ConnectionType type) noexcept
{
    Q_ASSERT(metaObj != nullptr);
    QString beanName = McPrivate::getBeanName(metaObj);
    connect(beanName, sender, signal, receiver, slot, type);
}

QMetaObject::Connection connect(IMcApplicationContext *appCtx, const QString &sender, const QString &signal,
    const QString &receiver, const QString &slot, Qt::ConnectionType type) noexcept
{
    Q_ASSERT(appCtx != nullptr);
    auto receiverObj = Mc::getObject(appCtx, receiver);
    if (receiverObj == nullptr) {
        return QMetaObject::Connection();
    }
    return connect(appCtx, sender, signal, receiverObj, slot, type);
}

QMetaObject::Connection connect(IMcApplicationContext *appCtx, const QString &sender, const QString &signal,
    QObject *receiver, const QString &slot, Qt::ConnectionType type) noexcept
{
    Q_ASSERT(appCtx != nullptr);
    Q_ASSERT(receiver != nullptr);
    auto senderObj = Mc::getObject(appCtx, sender);
    if (senderObj == nullptr) {
        return QMetaObject::Connection();
    }
    QString signalStr = signal;
    if (!signalStr.startsWith(QString::number(QSIGNAL_CODE))) {
        signalStr = QString::number(QSIGNAL_CODE) + signalStr;
    }
    auto slotStr = slot;
    if (!slotStr.startsWith(QString::number(QSLOT_CODE))) {
        slotStr = QString::number(QSLOT_CODE) + slotStr;
    }
    return QObject::connect(senderObj, signalStr.toLocal8Bit(), receiver, slotStr.toLocal8Bit(), type);
}

bool disconnect(IMcApplicationContext *appCtx, const QString &sender, const QString &signal, const QString &receiver,
    const QString &slot) noexcept
{
    Q_ASSERT(appCtx != nullptr);
    auto receiverObj = Mc::getObject(appCtx, receiver);
    return disconnect(appCtx, sender, signal, receiverObj, slot);
}

bool disconnect(IMcApplicationContext *appCtx, const QString &sender, const QString &signal, QObject *receiver,
    const QString &slot) noexcept
{
    Q_ASSERT(appCtx != nullptr);
    auto senderObj = Mc::getObject(appCtx, sender);
    if (senderObj == nullptr) {
        return false;
    }
    const char *sig = nullptr;
    const char *slt = nullptr;
    QString signalStr = signal;
    if (!signalStr.isEmpty() && !signalStr.startsWith(QString::number(QSIGNAL_CODE))) {
        signalStr = QString::number(QSIGNAL_CODE) + signalStr;
    }
    if (!signalStr.isEmpty()) {
        sig = signalStr.toLocal8Bit();
    }
    auto slotStr = slot;
    if (!slotStr.isEmpty() && !slotStr.startsWith(QString::number(QSLOT_CODE))) {
        slotStr = QString::number(QSLOT_CODE) + slotStr;
    }
    if (!slotStr.isEmpty()) {
        slt = slotStr.toLocal8Bit();
    }
    return QObject::disconnect(senderObj, sig, receiver, slt);
}
} // namespace Ioc
} // namespace Mc
