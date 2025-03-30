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
#include "McEventDispatcher.h"

#include "McEventRouter.h"

static constexpr const QChar s_segment = u'.';

MC_DECL_PRIVATE_DATA(McEventDispatcher)
McEventRouter router;
MC_DECL_PRIVATE_DATA_END

McEventDispatcher::McEventDispatcher() noexcept
{
    MC_NEW_PRIVATE_DATA(McEventDispatcher);
}

McEventDispatcher::~McEventDispatcher() {}

QMetaObject::Connection McEventDispatcher::connectToEvent(
    const QString &eventSpec, const QObject *receiver, const char *method, Qt::ConnectionType type) noexcept
{
    return d->router.connectToEvent(eventSpec.split(s_segment), receiver, method, type);
}

bool McEventDispatcher::disconnectEvent(const QString &eventSpec, const QObject *receiver, const char *method) noexcept
{
    if (eventSpec.isEmpty()) {
        return d->router.disconnect(SIGNAL(eventOccurred(QVariantList)), receiver, method);
    } else {
        return d->router.disconnectEvent(eventSpec.split(s_segment), receiver, method);
    }
}

bool McEventDispatcher::disconnectEvent(const QString &eventSpec, const QObject *receiver) noexcept
{
    if (eventSpec.isEmpty()) {
        return d->router.disconnect(&d->router, &McEventRouter::eventOccurred, receiver, nullptr);
    } else {
        return d->router.disconnectEvent(eventSpec.split(s_segment), receiver);
    }
}

void McEventDispatcher::submitEvent_helper(const QString &eventName, const QVariantList &datas) noexcept
{
    d->router.route(eventName.split(s_segment), datas);
}

QMetaObject::Connection McEventDispatcher::connectToEventImpl(
    const QString &eventSpec, const McSlotObjectWrapper &slotObject, Qt::ConnectionType type) noexcept
{
    return d->router.connectToEvent(eventSpec.split(s_segment), slotObject, type);
}

namespace Mc {
McEventDispatcher &eventDispatcher() noexcept
{
    static McEventDispatcher ins;
    return ins;
}
} // namespace Mc
