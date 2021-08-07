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

#include "../../Utils/McCancel.h"
#include "../../Utils/McProgress.h"

MC_FORWARD_DECL_CLASS(IMcResponseHandler);

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractResponse);

/*!
 * \brief The McAbstractResponse class
 * 此抽象类的子类在每次请求时都会构造一个新的对象，
 * 可以设计成使用对象池来优化
 */
class MCQUICKBOOT_EXPORT McAbstractResponse : public QObject
{
    Q_OBJECT
    MC_DECL_INIT(McAbstractResponse)
public:
    explicit McAbstractResponse(QObject *parent = nullptr);
    ~McAbstractResponse() override;

    void setHandlers(const QList<IMcResponseHandlerPtr> &val) noexcept;

    Q_INVOKABLE void cancel() noexcept;
    Q_INVOKABLE bool isCanceled() const noexcept;

    QVariant body() const noexcept;
    void setBody(const QVariant &var) noexcept;

    bool isAsyncCall() const noexcept;
    void setAsyncCall(bool val) noexcept;

    Q_INVOKABLE void attach(QObject *obj) noexcept;
    Q_INVOKABLE void detach() noexcept;

protected:
    void customEvent(QEvent *event) override;

    virtual void callCallback() noexcept = 0;
    virtual void callError() noexcept = 0;

    McProgress &getProgress() const noexcept;
    McCancel &getCancel() const noexcept;

private:
    void call() noexcept;

private:
    MC_DECL_PRIVATE(McAbstractResponse)

    friend class McRequestRunner;
};

Q_DECLARE_METATYPE(McAbstractResponse *)
