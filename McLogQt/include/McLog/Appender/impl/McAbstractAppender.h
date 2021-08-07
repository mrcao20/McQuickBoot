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

#include "../IMcConfigurableAppender.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractAppender);

class MCLOGQT_EXPORT McAbstractAppender 
        : public QObject
        , public IMcConfigurableAppender
        
{
    Q_OBJECT
    MC_DECL_INIT(McAbstractAppender)
    MC_TYPELIST(QObject, IMcConfigurableAppender)
    Q_PROPERTY(QString threshold READ threshold WRITE setThreshold)
public:
    McAbstractAppender();
    ~McAbstractAppender() override;
    
    QString threshold() const noexcept override;
    void setThreshold(const QString &val) noexcept override;
    
    QList<QtMsgType> types() const noexcept override;

    Q_INVOKABLE
    MC_BEAN_FINISHED
    void finished() noexcept;

    Q_INVOKABLE
    MC_THREAD_FINISHED
    void threadFinished() noexcept;

    Q_INVOKABLE
    MC_ALL_FINISHED
    void allFinished() noexcept;

protected:
    virtual void doFinished() noexcept {}
    virtual void doThreadFinished() noexcept {}
    virtual void doAllFinished() noexcept {}

private:
    QList<QtMsgType> initThreshold(const QString &val) const noexcept;
    int strToEnum(const QString &val) const noexcept;
    
private:
    MC_DECL_PRIVATE(McAbstractAppender)
};

MC_DECL_METATYPE(McAbstractAppender)
