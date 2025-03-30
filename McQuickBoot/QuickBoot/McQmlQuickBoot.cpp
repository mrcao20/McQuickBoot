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
#include "McQmlQuickBoot.h"

#include <McIoc/ApplicationContext/IMcApplicationContext.h>

#include "../Invoker/McQmlInvoker.h"

MC_GLOBAL_STATIC_BEGIN(staticData)
std::function<void(QCoreApplication *)> preInitFunc;
std::function<void(QCoreApplication *, QQmlApplicationEngine *)> afterInitFunc;
MC_GLOBAL_STATIC_END(staticData)

MC_DECL_PRIVATE_DATA2(McQmlQuickBoot)
{
    QQmlApplicationEngine *engine{nullptr};
    McQmlInvoker *invoker{nullptr};
};

McQmlQuickBoot::McQmlQuickBoot(QObject *parent) noexcept
    : McAbstractQuickBoot(parent)
{
    MC_NEW_PRIVATE_DATA(McQmlQuickBoot);
}

McQmlQuickBoot::~McQmlQuickBoot() {}

void McQmlQuickBoot::init(QCoreApplication *app, QQmlApplicationEngine *engine) noexcept
{
    if (staticData->preInitFunc != nullptr) {
        staticData->preInitFunc(app);
    }

    auto boot = McQmlQuickBootPtr::create();
    boot->d->engine = engine;
    setInstance(boot);
    boot->loadContext(QStringList() << QStringLiteral("qmlInvoker"));

    if (staticData->afterInitFunc != nullptr) {
        staticData->afterInitFunc(app, engine);
    }
}

McQmlQuickBootPtr McQmlQuickBoot::instance() noexcept
{
    auto ins = super::instance();
    auto qmlIns = ins.objectCast<McQmlQuickBoot>();
    return qmlIns;
}

void McQmlQuickBoot::setPreInitFunc(const std::function<void(QCoreApplication *)> &func) noexcept
{
    staticData->preInitFunc = func;
}

void McQmlQuickBoot::setAfterInitFunc(
    const std::function<void(QCoreApplication *, QQmlApplicationEngine *)> &func) noexcept
{
    staticData->afterInitFunc = func;
}

void McQmlQuickBoot::loadInvoker(IMcApplicationContext *appCtx, QThread *workThread) noexcept
{
    d->invoker = appCtx->getBean<McQmlInvoker *>(QStringLiteral("qmlInvoker"), workThread);
}

void McQmlQuickBoot::flushInvoker(IMcApplicationContext *appCtx) noexcept
{
    d->invoker->setAppCtx(appCtx);

    //! engine的newQObject函数会将其参数所有权转移到其返回的QJSValue中
    QJSValue jsObj = d->engine->newQObject(d->invoker);
    d->engine->globalObject().setProperty('$', jsObj);
    d->engine->importModule(QStringLiteral(":/invoker.mjs"));
}
