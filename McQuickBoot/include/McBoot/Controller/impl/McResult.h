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

#include "../../McBootGlobal.h"

MC_FORWARD_DECL_PRIVATE_DATA(McResult);

class MCQUICKBOOT_EXPORT McResult : public QObject 
{
    Q_OBJECT
    MC_DECL_INIT(McResult)
    MC_JSON_SERIALIZATION()
    Q_PROPERTY(bool success READ isSuccess)
    Q_PROPERTY(QString errMsg READ errMsg)
    Q_PROPERTY(QVariant result READ result)
public:
    explicit McResult(QObject *parent = nullptr);
    ~McResult() override;
    
    bool isSuccess() const noexcept;
    void setSuccess(bool val) noexcept;
    
    QString errMsg() const noexcept;
    void setErrMsg(const QString &val) noexcept;
    
    QVariant result() const noexcept;
    void setResult(const QVariant &val) noexcept;

    static QSharedPointer<McResult> ok(const QVariant &val) noexcept
    {
        auto result = QSharedPointer<McResult>::create();
        result->setSuccess(true);
        result->setResult(val);
        return result;
    }

    static QSharedPointer<McResult> fail(const QString &val) noexcept
    {
        auto result = QSharedPointer<McResult>::create();
        result->setSuccess(false);
        result->setErrMsg(val);
        return result;
    }

    friend MCQUICKBOOT_EXPORT QDebug operator<<(QDebug dbg, McResult *r);
    friend MCQUICKBOOT_EXPORT QDebug operator<<(QDebug dbg, const QSharedPointer<McResult> &r);

private:
    bool isInternalError() const noexcept;
    void setInternalError(bool val) noexcept;

private:
    MC_DECL_PRIVATE(McResult)

    friend class McControllerContainer;
    friend class McAbstractResponse;
};

MC_DECL_METATYPE(McResult)
