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
#include "McEventDispatcher.h"

#include "McEventRouter.h"

MC_DECL_PRIVATE_DATA(McEventDispatcher)
McEventRouter router;
MC_DECL_PRIVATE_DATA_END

McEventDispatcher::McEventDispatcher() noexcept
{
    MC_NEW_PRIVATE_DATA(McEventDispatcher);
}

McEventDispatcher *McEventDispatcher::instance() noexcept
{
    static McEventDispatcher ins;
    return &ins;
}

QMetaObject::Connection McEventDispatcher::connectToEvent(const QString &scxmlEventSpec,
                                                          const QObject *receiver,
                                                          const char *method,
                                                          Qt::ConnectionType type) noexcept
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

QMetaObject::Connection McEventDispatcher::connectToEventImpl(const QString &scxmlEventSpec,
                                                              const QObject *receiver,
                                                              void **slot,
                                                              QtPrivate::QSlotObjectBase *slotObj,
                                                              Qt::ConnectionType type) noexcept
{
    return d->router.connectToEvent(scxmlEventSpec.split(QLatin1Char('.')),
                                    receiver,
                                    slot,
                                    slotObj,
                                    type);
}
