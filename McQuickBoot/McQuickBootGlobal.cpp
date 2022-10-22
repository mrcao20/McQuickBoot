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
#include "McQuickBootGlobal.h"

#include <QThreadPool>

Q_LOGGING_CATEGORY(mcQuickBoot, "mc.quickboot")

MC_GLOBAL_STATIC_BEGIN(staticData)
bool waitThreadPoolDone{true}; //!< 是否等待线程池退出
int threadPoolWaitTimeout{-1}; //!< 等待线程池退出的超时时长，-1为永不超时，直至线程池完全退出
QThreadPool globalThreadPool;
MC_GLOBAL_STATIC_END(staticData)

MC_DESTROY2(Mc::RoutineQuickBootThreadPool)
{
    if (!staticData.exists()) {
        return;
    }
    if (staticData->waitThreadPoolDone) {
        staticData->globalThreadPool.waitForDone(staticData->threadPoolWaitTimeout);
    }
}

namespace Mc {
QThreadPool *globalThreadPool() noexcept
{
    return &staticData->globalThreadPool;
}

void setWaitThreadPoolDone(bool val) noexcept
{
    staticData->waitThreadPoolDone = val;
}

void setThreadPoolWaitTimeout(int val) noexcept
{
    staticData->threadPoolWaitTimeout = val;
}

void setMaxThreadCount(int val) noexcept
{
    staticData->globalThreadPool.setMaxThreadCount(val);
}
} // namespace Mc
