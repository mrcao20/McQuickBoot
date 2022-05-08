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
