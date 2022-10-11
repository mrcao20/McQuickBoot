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

#include <QIODevice>

#include "../McLogGlobal.h"

class IMcCodecableAppender;

MC_FORWARD_DECL_PRIVATE_DATA(McVSDebugDevice);

class MC_LOG_EXPORT McVSDebugDevice : public QIODevice
{
    Q_OBJECT
public:
    explicit McVSDebugDevice(IMcCodecableAppender *codecableAppender, QObject *parent = nullptr) noexcept;
    ~McVSDebugDevice() override;

protected:
    qint64 readData(char *data, qint64 maxSize) override;
    qint64 writeData(const char *data, qint64 maxSize) override;

private:
    MC_DECL_PRIVATE(McVSDebugDevice)
};

MC_DECL_METATYPE(McVSDebugDevice)
