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

MC_DECL_PRIVATE_DATA(McEventDispatcher)
McEventRouter router;
MC_DECL_PRIVATE_DATA_END

McEventDispatcher::McEventDispatcher() noexcept
{
    MC_NEW_PRIVATE_DATA(McEventDispatcher);
}

McEventDispatcher::~McEventDispatcher() {}

McEventDispatcher *McEventDispatcher::instance() noexcept
{
    static McEventDispatcher ins;
    return &ins;
}

QMetaObject::Connection McEventDispatcher::connectToEvent(
    const QString &scxmlEventSpec, const QObject *receiver, const char *method, Qt::ConnectionType type) noexcept
{
    return d->router.connectToEvent(scxmlEventSpec.split(QLatin1Char('.')), receiver, method, type);
}

void McEventDispatcher::submitEvent(const QString &eventName) noexcept
{
    submitEvent_helper(eventName, QVariant());
}

void McEventDispatcher::submitEvent_helper(const QString &eventName, const QVariant &data) noexcept
{
    d->router.route(eventName.split(QLatin1Char('.')), data);
}

QMetaObject::Connection McEventDispatcher::connectToEventImpl(
    const QString &scxmlEventSpec, const McSlotObjectWrapper &slotObject, Qt::ConnectionType type) noexcept
{
    return d->router.connectToEvent(scxmlEventSpec.split(QLatin1Char('.')), slotObject, type);
}

namespace Mc {
McEventDispatcher &eventDispatcher() noexcept
{
    return *McEventDispatcher::instance();
}
} // namespace Mc
