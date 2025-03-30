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
#include "McCppQuickBoot.h"

#include <McIoc/ApplicationContext/IMcApplicationContext.h>

MC_GLOBAL_STATIC_BEGIN(staticData)
std::function<void(QCoreApplication *)> preInitFunc;
std::function<void(QCoreApplication *)> afterInitFunc;
MC_GLOBAL_STATIC_END(staticData)

MC_DECL_PRIVATE_DATA2(McCppQuickBoot)
{
    McCppInvokerPtr invoker;
};

McCppQuickBoot::McCppQuickBoot(QObject *parent) noexcept
    : McAbstractQuickBoot(parent)
{
    MC_NEW_PRIVATE_DATA(McCppQuickBoot);
}

McCppQuickBoot::~McCppQuickBoot() {}

void McCppQuickBoot::init(QCoreApplication *app) noexcept
{
    if (staticData->preInitFunc != nullptr) {
        staticData->preInitFunc(app);
    }

    auto boot = McCppQuickBootPtr::create();
    setInstance(boot);
    boot->loadContext();

    if (staticData->afterInitFunc != nullptr) {
        staticData->afterInitFunc(app);
    }
}

McCppQuickBootPtr McCppQuickBoot::instance() noexcept
{
    auto ins = super::instance();
    auto cppIns = ins.objectCast<McCppQuickBoot>();
    return cppIns;
}

void McCppQuickBoot::setPreInitFunc(const std::function<void(QCoreApplication *)> &func) noexcept
{
    staticData->preInitFunc = func;
}

void McCppQuickBoot::setAfterInitFunc(const std::function<void(QCoreApplication *)> &func) noexcept
{
    staticData->afterInitFunc = func;
}

McCppInvoker &McCppQuickBoot::invoker() const noexcept
{
    return *d->invoker.data();
}

void McCppQuickBoot::loadInvoker(IMcApplicationContext *appCtx, QThread *workThread) noexcept
{
    d->invoker = appCtx->getBean<McCppInvokerPtr>(QStringLiteral("cppInvoker"), workThread);
}

void McCppQuickBoot::flushInvoker(IMcApplicationContext *appCtx) noexcept
{
    d->invoker->setAppCtx(appCtx);
}
