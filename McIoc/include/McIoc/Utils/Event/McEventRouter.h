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
#pragma once

#include <QHash>
#include <QObject>

class McEventRouter : public QObject
{
    Q_OBJECT
public:
    explicit McEventRouter(QObject *parent = nullptr) : QObject(parent) {}

    QMetaObject::Connection connectToEvent(const QStringList &segments,
                                           const QObject *receiver,
                                           const char *method,
                                           Qt::ConnectionType type);
    QMetaObject::Connection connectToEvent(const QStringList &segments,
                                           const QObject *receiver,
                                           void **slot,
                                           QtPrivate::QSlotObjectBase *method,
                                           Qt::ConnectionType type);

    void route(const QStringList &segments, const QVariant &data);

signals:
    void eventOccurred(const QVariant &data);

private:
    McEventRouter *child(const QString &segment);

private:
    QHash<QString, McEventRouter *> children;
};
