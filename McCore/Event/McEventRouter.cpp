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

namespace {

static QString nextSegment(const QStringList &segments) noexcept
{
    if (segments.isEmpty())
        return QString();

    const QString &segment = segments.first();
    return segment == QLatin1String("*") ? QString() : segment;
}

} // namespace

QMetaObject::Connection McEventRouter::connectToEvent(
    const QStringList &segments, const QObject *receiver, const char *method, Qt::ConnectionType type) noexcept
{
    QString segment = nextSegment(segments);
    return segment.isEmpty() ? connect(this, SIGNAL(eventOccurred(QVariant)), receiver, method, type)
                             : child(segment)->connectToEvent(segments.mid(1), receiver, method, type);
}

QMetaObject::Connection McEventRouter::connectToEvent(
    const QStringList &segments, const McSlotObjectWrapper &slotObject, Qt::ConnectionType type) noexcept
{
    QString segment = nextSegment(segments);
    if (segment.isEmpty()) {
        auto receiver = slotObject.recever();
        if (receiver == nullptr) {
            receiver = this;
        }
        return connect(
            this, &McEventRouter::eventOccurred, receiver,
            [slotObject = std::move(slotObject)](const QVariant &data) { slotObject.call(data); }, type);
    } else {
        return child(segment)->connectToEvent(segments.mid(1), slotObject, type);
    }
}

void McEventRouter::route(const QStringList &segments, const QVariant &data) noexcept
{
    Q_EMIT eventOccurred(data);
    if (!segments.isEmpty()) {
        auto it = m_children.find(segments.first());
        if (it != m_children.end())
            it.value()->route(segments.mid(1), data);
    }
}

McEventRouter *McEventRouter::child(const QString &segment) noexcept
{
    McEventRouter *&child = m_children[segment];
    if (child == nullptr)
        child = new McEventRouter(this);
    return child;
}
