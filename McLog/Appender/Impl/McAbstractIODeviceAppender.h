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

#include "../IMcCodecableAppender.h"
#include "../IMcWritableAppender.h"
#include "McAbstractAppender.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractIODeviceAppender)

class MC_LOG_EXPORT McAbstractIODeviceAppender
    : public McAbstractAppender
    , public IMcWritableAppender
    , public IMcCodecableAppender
{
    Q_OBJECT
    MC_DECL_SUPER(McAbstractAppender)
    MC_INTERFACES(McAbstractAppender, IMcWritableAppender, IMcCodecableAppender)
    Q_PROPERTY(QByteArray codecName READ codecName WRITE setCodecName)
public:
    McAbstractIODeviceAppender() noexcept;
    ~McAbstractIODeviceAppender();

    QIODevicePtr device() const noexcept;
    void setDevice(const QIODevicePtr &device) noexcept override;
    QByteArray codecName() const noexcept;
    void setCodecName(const QByteArray &val) noexcept;
    QStringConverter::Encoding encoding() const noexcept override;
    void setEncoding(QStringConverter::Encoding val) noexcept override;

    void buildCompleted() noexcept override;

protected:
    QTextStream &textStream() noexcept;

private:
    MC_DECL_PRIVATE(McAbstractIODeviceAppender)
};

MC_DECL_POINTER(McAbstractIODeviceAppender)
