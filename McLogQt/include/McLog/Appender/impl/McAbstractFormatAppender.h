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

#include "McAbstractIODeviceAppender.h"

MC_FORWARD_DECL_CLASS(IMcLayout);

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractFormatAppender);

class MCLOGQT_EXPORT McAbstractFormatAppender
        : public McAbstractIODeviceAppender
{
    Q_OBJECT
    MC_DECL_INIT(McAbstractFormatAppender)
    MC_TYPELIST(McAbstractIODeviceAppender)
    Q_PROPERTY(IMcLayoutPtr layout READ layout WRITE setLayout)
    Q_PROPERTY(bool immediateFlush READ immediateFlush WRITE setImmediateFlush)
public:
    McAbstractFormatAppender();
    ~McAbstractFormatAppender() override;
    
    IMcLayoutPtr layout() const noexcept;
    void setLayout(IMcLayoutConstPtrRef val) noexcept;
    
    bool immediateFlush() const noexcept;
    void setImmediateFlush(bool val) noexcept;
    
    void append(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept override;

protected:
    void doFinished() noexcept override;
    void doThreadFinished() noexcept override;

    void customEvent(QEvent *event) override;
    
    virtual void writeBefore() noexcept = 0;
    virtual void writeAfter() noexcept = 0;
    
private:
    Q_INVOKABLE void append_helper(const QString &msg) noexcept;
    
private:
    MC_DECL_PRIVATE(McAbstractFormatAppender)
};

MC_DECL_METATYPE(McAbstractFormatAppender)
