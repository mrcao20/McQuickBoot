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

#include "McAbstractRequestor.h"

#include <QJSValue>

MC_FORWARD_DECL_PRIVATE_DATA(McQmlRequestor);

class McQmlResponse;

class MCQUICKBOOT_EXPORT McQmlRequestor : public McAbstractRequestor
{
    Q_OBJECT
    MC_DECL_INIT(McQmlRequestor)
    MC_TYPELIST(McAbstractRequestor)
    MC_COMPONENT("qmlRequestor")
    MC_POINTER(true)
public:
    using McAbstractRequestor::connect;
    using McAbstractRequestor::disconnect;

    Q_INVOKABLE explicit McQmlRequestor(QObject *parent = nullptr);
    ~McQmlRequestor() override;

    Q_INVOKABLE McQmlResponse *invoke(const QString &uri,
                                      const QJSValue &data1 = QJSValue(),
                                      const QJSValue &data2 = QJSValue()) noexcept;
    Q_INVOKABLE QJSValue syncInvoke(const QString &uri) noexcept;
    Q_INVOKABLE QJSValue syncInvoke(const QString &uri, const QJSValue &data) noexcept;
    Q_INVOKABLE QString connect(const QString &beanName,
                                const QString &signal,
                                const QJSValue &callback) noexcept;
    Q_INVOKABLE void disconnect(const QString &uuid) noexcept;
    Q_INVOKABLE void disconnect(const QString &beanName,
                                const QString &signal,
                                const QJSValue &callback = QJSValue()) noexcept;

private:
    MC_DECL_PRIVATE(McQmlRequestor)
};

MC_DECL_METATYPE(McQmlRequestor)
