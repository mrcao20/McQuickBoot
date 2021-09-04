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

#include "../McBootGlobal.h"

MC_FORWARD_DECL_PRIVATE_DATA(McRequestorConfig);

class McRequestorConfig : public QObject
{
    Q_OBJECT
    MC_DECL_INIT(McRequestorConfig)
    MC_COMPONENT("requestorConfig")
    MC_CONFIGURATION_PROPERTIES("boot.application.requestor")
    Q_PROPERTY(int maxThreadCount READ maxThreadCount WRITE setMaxThreadCount)
    Q_PROPERTY(bool autoIncrease READ autoIncrease WRITE setAutoIncrease)
    Q_PROPERTY(bool waitThreadPoolDone READ waitThreadPoolDone WRITE setWaitThreadPoolDone)
public:
    Q_INVOKABLE McRequestorConfig(QObject *parent = nullptr) noexcept;
    ~McRequestorConfig();

    int maxThreadCount() const noexcept;
    void setMaxThreadCount(int val) noexcept;

    bool autoIncrease() const noexcept;
    void setAutoIncrease(bool val) noexcept;

    bool waitThreadPoolDone() const noexcept;
    void setWaitThreadPoolDone(bool val) noexcept;

private:
    MC_DECL_PRIVATE(McRequestorConfig)
};

MC_DECL_METATYPE(McRequestorConfig)
