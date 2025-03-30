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
#include "McQuickBoot.h"

#include <McIoc/ApplicationContext/IMcApplicationContext.h>

#include "../Invoker/McQmlInvoker.h"

MC_GLOBAL_STATIC_BEGIN(staticData)
std::function<void(QCoreApplication *)> preInitFunc;
std::function<void(QCoreApplication *, QQmlApplicationEngine *)> afterInitFunc;
MC_GLOBAL_STATIC_END(staticData)

MC_DECL_PRIVATE_DATA2(McQuickBoot)
{
    QQmlApplicationEngine *engine{nullptr};
    McQmlInvoker *qmlInvoker{nullptr};
    McCppInvokerPtr cppInvoker;
};

McQuickBoot::McQuickBoot(QObject *parent) noexcept
    : McAbstractQuickBoot(parent)
{
    MC_NEW_PRIVATE_DATA(McQuickBoot);
}

McQuickBoot::~McQuickBoot() {}

void McQuickBoot::init(QCoreApplication *app, QQmlApplicationEngine *engine) noexcept
{
    if (staticData->preInitFunc != nullptr) {
        staticData->preInitFunc(app);
    }

    auto boot = McQuickBootPtr::create();
    boot->d->engine = engine;
    setInstance(boot);
    boot->loadContext(QStringList() << QStringLiteral("qmlInvoker"));

    if (staticData->afterInitFunc != nullptr) {
        staticData->afterInitFunc(app, engine);
    }
}

McQuickBootPtr McQuickBoot::instance() noexcept
{
    auto ins = super::instance();
    auto qmlIns = ins.objectCast<McQuickBoot>();
    return qmlIns;
}

void McQuickBoot::setPreInitFunc(const std::function<void(QCoreApplication *)> &func) noexcept
{
    staticData->preInitFunc = func;
}

void McQuickBoot::setAfterInitFunc(const std::function<void(QCoreApplication *, QQmlApplicationEngine *)> &func) noexcept
{
    staticData->afterInitFunc = func;
}

McCppInvoker &McQuickBoot::invoker() const noexcept
{
    return *d->cppInvoker.data();
}

void McQuickBoot::loadInvoker(IMcApplicationContext *appCtx, QThread *workThread) noexcept
{
    d->qmlInvoker = appCtx->getBean<McQmlInvoker *>(QStringLiteral("qmlInvoker"), workThread);
    d->cppInvoker = appCtx->getBean<McCppInvokerPtr>(QStringLiteral("cppInvoker"), workThread);
}

void McQuickBoot::flushInvoker(IMcApplicationContext *appCtx) noexcept
{
    d->qmlInvoker->setAppCtx(appCtx);
    d->cppInvoker->setAppCtx(appCtx);

    if (d->engine != nullptr) {
        //! engine的newQObject函数会将其参数所有权转移到其返回的QJSValue中
        QJSValue jsObj = d->engine->newQObject(d->qmlInvoker);
        d->engine->globalObject().setProperty(QStringLiteral("$"), jsObj);
        d->engine->importModule(QStringLiteral(":/invoker.mjs"));
    }
}
