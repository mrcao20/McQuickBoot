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
#include "McEventRouter.h"

static QString nextSegment(const QStringList &segments) noexcept
{
    if (segments.isEmpty())
        return QString();

    const QString &segment = segments.constFirst();
    return segment == u'*' ? QString() : segment;
}

McEventRouter::McEventRouter(QObject *parent) noexcept
    : QObject(parent)
{
}

QMetaObject::Connection McEventRouter::connectToEvent(
    const QStringList &segments, const QObject *receiver, const char *method, Qt::ConnectionType type) noexcept
{
    QString segment = nextSegment(segments);
    return segment.isEmpty() ? connect(this, SIGNAL(eventOccurred(QVariantList)), receiver, method, type)
                             : child(segment)->connectToEvent(segments.mid(1), receiver, method, type);
}

QMetaObject::Connection McEventRouter::connectToEvent(
    const QStringList &segments, const McSlotObjectWrapper &slotObject, Qt::ConnectionType type) noexcept
{
    QString segment = nextSegment(segments);
    if (segment.isEmpty()) {
        auto receiver = slotObject.receiver();
        if (receiver == nullptr) {
            receiver = this;
        }
        return connect(
            this, &McEventRouter::eventOccurred, receiver,
            [slotObject = std::move(slotObject)](const QVariantList &datas) { slotObject.call(datas); }, type);
    } else {
        return child(segment)->connectToEvent(segments.mid(1), slotObject, type);
    }
}

bool McEventRouter::disconnectEvent(const QStringList &segments, const QObject *receiver, const char *method) noexcept
{
    if (segments.isEmpty()) {
        return false;
    }
    QString segment = segments.constFirst();
    if (!m_children.contains(segment)) {
        return false;
    }
    QStringList childSegments = segments.mid(1);
    QString nSeg = nextSegment(childSegments);
    if (nSeg.isEmpty()) {
        auto router = m_children.take(segment);
        bool flag = router->disconnect(router, SIGNAL(eventOccurred(QVariantList)), receiver, method);
        router->deleteLater();
        return flag;
    }
    return child(segment)->disconnectEvent(childSegments, receiver, method);
}

bool McEventRouter::disconnectEvent(const QStringList &segments, const QObject *receiver) noexcept
{
    if (segments.isEmpty()) {
        return false;
    }
    QString segment = segments.constFirst();
    if (!m_children.contains(segment)) {
        return false;
    }
    QStringList childSegments = segments.mid(1);
    QString nSeg = nextSegment(childSegments);
    if (nSeg.isEmpty()) {
        auto router = m_children.take(segment);
        bool flag = router->disconnect(router, &McEventRouter::eventOccurred, receiver, nullptr);
        router->deleteLater();
        return flag;
    }
    return child(segment)->disconnectEvent(childSegments, receiver);
}

void McEventRouter::route(const QStringList &segments, const QVariantList &datas) noexcept
{
    Q_EMIT eventOccurred(datas);
    if (!segments.isEmpty()) {
        auto it = m_children.constFind(segments.constFirst());
        if (it != m_children.constEnd())
            it.value()->route(segments.mid(1), datas);
    }
}

McEventRouter *McEventRouter::child(const QString &segment) noexcept
{
    McEventRouter *&child = m_children[segment];
    if (child == nullptr)
        child = new McEventRouter(this);
    return child;
}
