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

#include "McAbstractResponse.h"

QT_BEGIN_NAMESPACE
class QJSValue;
class QJSEngine;
QT_END_NAMESPACE

MC_FORWARD_DECL_PRIVATE_DATA(McQmlResponse);

class MCQUICKBOOT_EXPORT McQmlResponse : public McAbstractResponse
{
    Q_OBJECT
    MC_DECL_INIT(McQmlResponse)
public:
    explicit McQmlResponse(QObject *parent = nullptr);
    ~McQmlResponse() override;
    
    Q_INVOKABLE McQmlResponse *then(const QJSValue &callback) noexcept;
    Q_INVOKABLE McQmlResponse *syncThen(const QJSValue &callback) noexcept;
    Q_INVOKABLE McQmlResponse *asyncThen(const QJSValue &callback) noexcept;
    Q_INVOKABLE McQmlResponse *error(const QJSValue &func) noexcept;
    Q_INVOKABLE McQmlResponse *progress(const QJSValue &callback) noexcept;

protected:
    void callCallback() noexcept override;
    void callError() noexcept override;

private:
    void call(QJSValue &func) noexcept;

private:
    MC_DECL_PRIVATE(McQmlResponse)
};

Q_DECLARE_METATYPE(McQmlResponse *)
