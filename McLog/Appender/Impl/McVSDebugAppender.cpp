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
#include "McVSDebugAppender.h"

#include "Device/McVSDebugDevice.h"

MC_AUTO_INIT(McVSDebugAppender)
MC_INIT_END

void McVSDebugAppender::buildFinished() noexcept
{
    super::buildFinished();

    auto dev = McVSDebugDevicePtr::create(this);
    if (!dev->open(QIODevice::WriteOnly)) {
        MC_PRINT_ERR("error open vs debuger for write!!!\n");
        return;
    }

    setDevice(dev);
}

void McVSDebugAppender::writeBefore() noexcept {}

void McVSDebugAppender::writeAfter() noexcept {}
