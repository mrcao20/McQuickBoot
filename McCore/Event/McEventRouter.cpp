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

#include <private/qmetaobject_p.h>
#include <private/qobject_p.h>

namespace {

static int signalIndex(const QMetaObject *meta, const QByteArray &signalName) noexcept
{
    Q_ASSERT(meta);

    int signalIndex = meta->indexOfSignal(signalName.constData());

    // If signal doesn't exist, return negative value
    if (signalIndex < 0)
        return signalIndex;

    // signal belongs to class whose meta object was passed, not some derived class.
    Q_ASSERT(meta->methodOffset() <= signalIndex);

    // Duplicate of computeOffsets in qobject.cpp
    const QMetaObject *m = meta->d.superdata;
    while (m) {
        const QMetaObjectPrivate *d = QMetaObjectPrivate::get(m);
        signalIndex = signalIndex - d->methodCount + d->signalCount;
        m = m->d.superdata;
    }

    // Asserting about the signal not being cloned would be nice, too, but not practical.

    return signalIndex;
}

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

QMetaObject::Connection McEventRouter::connectToEvent(const QStringList &segments, const QObject *receiver, void **slot,
    QtPrivate::QSlotObjectBase *method, Qt::ConnectionType type) noexcept
{
    QString segment = nextSegment(segments);
    if (segment.isEmpty()) {
        const int *types = nullptr;
        if (type == Qt::QueuedConnection || type == Qt::BlockingQueuedConnection)
            types = QtPrivate::ConnectionTypes<QtPrivate::List<QVariant>>::types();

        const QMetaObject *meta = metaObject();
        static const int eventOccurredIndex = signalIndex(meta, "eventOccurred(QVariant)");
        return QObjectPrivate::connectImpl(this, eventOccurredIndex, receiver, slot, method, type, types, meta);
    } else {
        return child(segment)->connectToEvent(segments.mid(1), receiver, slot, method, type);
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
