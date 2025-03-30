/*
 * Copyright (c) 2021 mrcao20/mrcao20@163.com
 * McQuickBoot is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
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
#ifdef MC_USE_QT5
    QTextCodec *codec() const noexcept override;
    void setCodec(QTextCodec *val) noexcept override;
#else
    QStringConverter::Encoding encoding() const noexcept override;
    void setEncoding(QStringConverter::Encoding val) noexcept override;
#endif

    void buildCompleted() noexcept override;

protected:
    QTextStream &textStream() noexcept;

private:
    MC_DECL_PRIVATE(McAbstractIODeviceAppender)
};

MC_DECL_POINTER(McAbstractIODeviceAppender)
