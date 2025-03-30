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
#include "McObjectBeanBuilder.h"

#include <QMetaMethod>
#include <QThread>

#include "../IMcBeanReferenceResolver.h"
#include "McBeanConnector.h"
#include "McBeanEnum.h"
#include "McBeanReference.h"
#include "Utils/IMcBeanBuildable.h"

MC_DECL_PRIVATE_DATA(McObjectBeanBuilder)
QList<McBeanConnectorPtr> connectors;
McBeanReferencePtr parentBeanReference;
MC_DECL_PRIVATE_DATA_END

McObjectBeanBuilder::McObjectBeanBuilder() noexcept
{
    MC_NEW_PRIVATE_DATA(McObjectBeanBuilder);
}

McObjectBeanBuilder::~McObjectBeanBuilder() {}

void McObjectBeanBuilder::addConnector(const McBeanConnectorPtr &val) noexcept
{
    d->connectors.append(val);
}

void McObjectBeanBuilder::setParentBeanReference(const McBeanReferencePtr &val) noexcept
{
    d->parentBeanReference = val;
}

void McObjectBeanBuilder::complete(QVariant &bean, QThread *thread) noexcept
{
    auto obj = bean.value<QObject *>();
    if (Q_UNLIKELY(obj == nullptr)) {
        return;
    }
    auto buildableObj = static_cast<IMcBeanBuildable *>(obj->qt_metacast("IMcBeanBuildable"));
    if (buildableObj == nullptr) {
        buildableObj = bean.value<IMcBeanBuildable *>();
    }
    auto properties = buildProperties(bean);
    callStartFunction(obj, buildableObj);
    addPropertyValue(obj, properties);
    addObjectConnect(obj, properties);
    callFinishedFunction(obj, buildableObj);
    if (thread != nullptr && thread != obj->thread()) {
        obj->moveToThread(thread);
        callThreadMovedFunction(obj, buildableObj);
    }
    Qt::ConnectionType conType = Qt::QueuedConnection;
    if (obj->thread() == QThread::currentThread()) {
        conType = Qt::DirectConnection;
    } else if (obj->thread() != QThread::currentThread() && obj->thread()->isRunning()) {
        conType = Qt::BlockingQueuedConnection;
    } else {
        qCCritical(mcIoc()) << "if you want to moved to other thread. please make sure call QThread::start "
                               "before call getBean/refresh.";
    }
    callCompletedFunction(obj, buildableObj, conType);
    if (!d->parentBeanReference.isNull()) {
        auto parentVar = resolver()->resolveBeanReference<QObject *>(d->parentBeanReference);
        setParent(obj, parentVar);
    }
}

void McObjectBeanBuilder::doMoveToThread(const QVariant &bean, QThread *thread, const QVariantHash &properties) noexcept
{
    if (Q_UNLIKELY(resolver() == nullptr)) {
        return;
    }
    auto obj = bean.value<QObject *>();
    if (Q_UNLIKELY(obj == nullptr)) {
        return;
    }
    if (obj->thread() != thread) {
        obj->moveToThread(thread);
    }
    for (auto &value : properties) {
#ifdef MC_USE_QT5
        if (value.userType() != qMetaTypeId<McBeanReferencePtr>()) {
#else
        if (value.metaType() != QMetaType::fromType<McBeanReferencePtr>()) {
#endif
            continue;
        }
        auto ref = value.value<McBeanReferencePtr>();
        resolver()->beanReferenceMoveToThread(ref, thread);
    }
}

void McObjectBeanBuilder::setParent(QObject *bean, QObject *parent) noexcept
{
    bean->setParent(parent);
}

void McObjectBeanBuilder::addPropertyValue(QObject *bean, const QVariantMap &pros)
{
    if (bean == nullptr) {
        return;
    }
    QMapIterator<QString, QVariant> itr(pros);
    while (itr.hasNext()) {
        auto item = itr.next();
        auto key = item.key();
        auto value = item.value();

        auto metaObj = bean->metaObject();
        auto index = metaObj->indexOfProperty(key.toLocal8Bit());
        if (index == -1) {
            qCDebug(mcIoc(), "bean '%s' cannot found property named for '%s'. it will be a dynamic property",
                metaObj->className(), qPrintable(key));
            bean->setProperty(key.toLocal8Bit(), value);

        } else {
            auto metaProperty = metaObj->property(index);
            if (Q_UNLIKELY(!metaProperty.write(bean, value))) {
                qCCritical(
                    mcIoc(), "bean '%s' write property named for '%s' failure", metaObj->className(), qPrintable(key));
            }
        }
    }
}

void McObjectBeanBuilder::addObjectConnect(QObject *bean, const QVariantMap &pros)
{
    for (auto &con : qAsConst(d->connectors)) {
        QString senderProName = con->sender();
        QObject *sender = getPropertyObject(bean, senderProName, pros);
        if (sender == nullptr) {
            continue;
        }
        auto signalMetaObj = sender->metaObject();

        QString signalStr = con->signal();
        if (signalStr.startsWith(QString::number(QSIGNAL_CODE))) {
            signalStr.remove(0, 1);
        }
        if (signalStr.isEmpty()) {
            qCCritical(mcIoc(), "signal is not exists");
            continue;
        }
        int signalIndex = signalMetaObj->indexOfSignal(signalStr.toLocal8Bit());
        if (signalIndex == -1) {
            qCCritical(mcIoc(), "not exists signal named '%s' for bean '%s'", qPrintable(signalStr),
                signalMetaObj->className());
            continue;
        }
        QMetaMethod signal = signalMetaObj->method(signalIndex);

        QString receiverProName = con->receiver();
        QObject *receiver = getPropertyObject(bean, receiverProName, pros);
        if (receiver == nullptr) {
            continue;
        }
        auto slotMetaObj = receiver->metaObject();

        QString slotStr = con->slot();
        if (slotStr.startsWith(QString::number(QSLOT_CODE))) {
            slotStr.remove(0, 1);
        }
        if (slotStr.isEmpty()) {
            qCCritical(mcIoc(), "slot is not exists");
            continue;
        }
        int slotIndex = slotMetaObj->indexOfMethod(slotStr.toLocal8Bit());
        if (slotIndex == -1) {
            qCCritical(
                mcIoc(), "not exists slot named '%s' for bean '%s'", qPrintable(slotStr), slotMetaObj->className());
            continue;
        }
        QMetaMethod slot = slotMetaObj->method(slotIndex);

        Qt::ConnectionType type = Qt::AutoConnection;
        if (!con->type().isNull()) {
            type = convertEnum(QVariant::fromValue(con->type()), QVariant()).value<Qt::ConnectionType>();
        }

        QObject::connect(sender, signal, receiver, slot, type);
    }
}

QObject *McObjectBeanBuilder::getPropertyObject(
    QObject *bean, const QString &proName, const QVariantMap &proValues) noexcept
{
    if (bean == nullptr) {
        return nullptr;
    }
    QObject *obj = nullptr;
    if (proName == Mc::Constant::Tag::Xml::THIS) {
        obj = bean;
    } else {
        if (!proValues.contains(proName)) {
            qCCritical(mcIoc(), "not found property named '%s' for bean '%s'", bean->metaObject()->className(),
                qPrintable(proName));
            return nullptr;
        }
        auto varPro = proValues[proName];
        obj = varPro.value<QObject *>();
    }
    return obj;
}

void McObjectBeanBuilder::callStartFunction(QObject *bean, IMcBeanBuildable *buildableBean) noexcept
{
    callTagFunction(bean, MC_STRINGIFY(MC_STARTED));
    if (buildableBean != nullptr) {
        buildableBean->buildStarted();
    }
}

void McObjectBeanBuilder::callFinishedFunction(QObject *bean, IMcBeanBuildable *buildableBean) noexcept
{
    callTagFunction(bean, MC_STRINGIFY(MC_FINISHED));
    if (buildableBean != nullptr) {
        buildableBean->buildFinished();
    }
}

void McObjectBeanBuilder::callThreadMovedFunction(QObject *bean, IMcBeanBuildable *buildableBean) noexcept
{
    callTagFunction(bean, MC_STRINGIFY(MC_THREAD_MOVED));
    if (buildableBean != nullptr) {
        buildableBean->buildThreadMoved();
    }
}

void McObjectBeanBuilder::callCompletedFunction(
    QObject *bean, IMcBeanBuildable *buildableBean, Qt::ConnectionType type) noexcept
{
    callTagFunction(bean, MC_STRINGIFY(MC_COMPLETED), type);
    if (buildableBean != nullptr) {
        QMetaObject::invokeMethod(
            bean, [buildableBean]() { buildableBean->buildCompleted(); }, type);
    }
}

void McObjectBeanBuilder::callTagFunction(QObject *bean, const char *tag, Qt::ConnectionType type) noexcept
{
    auto mo = bean->metaObject();
    for (int i = 0; i < mo->methodCount(); ++i) {
        auto method = mo->method(i);
        if (!Mc::isContainedTag(method.tag(), tag)) {
            continue;
        }
        //! 遍历当前对象的所有方法，调用所有被标记过的方法，从超基类开始到子类
        method.invoke(bean, type);
    }
}
