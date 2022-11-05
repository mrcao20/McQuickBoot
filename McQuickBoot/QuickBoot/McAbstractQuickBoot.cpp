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
#include "McAbstractQuickBoot.h"

#include <QThread>

#include <McCore/McGlobal.h>
#include <McIoc/ApplicationContext/IMcApplicationContext.h>

MC_GLOBAL_STATIC_BEGIN(staticData)
McAbstractQuickBootPtr boot;
MC_GLOBAL_STATIC_END(staticData)

MC_DECL_PRIVATE_DATA2(McAbstractQuickBoot)
{
    McCppInvokerPtr invoker;
    QThread *workThread{nullptr};
};

MC_INIT(McAbstractQuickBoot)
#ifdef MC_USE_QT5
qRegisterMetaType<QThread *>();
#endif
MC_DESTROY()
if (!staticData.exists()) {
    return;
}
if (staticData->boot->d->workThread != nullptr) {
    staticData->boot->d->workThread->quit();
    staticData->boot->d->workThread->wait();
    delete staticData->boot->d->workThread;
    staticData->boot->d->workThread = nullptr;
}
staticData->boot.reset();
MC_INIT_END

McAbstractQuickBoot::McAbstractQuickBoot(QObject *parent) noexcept
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractQuickBoot);
}

McAbstractQuickBoot::~McAbstractQuickBoot() {}

McAbstractQuickBootPtr McAbstractQuickBoot::instance() noexcept
{
    McAbstractQuickBootPtr &boot = staticData->boot;
    Q_ASSERT_X(!boot.isNull(), Q_FUNC_INFO, "please call 'init' before");
    return boot;
}

McCppInvoker &McAbstractQuickBoot::invoker() const noexcept
{
    return *d->invoker.data();
}

void McAbstractQuickBoot::setInstance(const McAbstractQuickBootPtr &ins) noexcept
{
    staticData->boot = ins;
}

void McAbstractQuickBoot::doRefresh(const QStringList &preloadBeans) noexcept
{
    auto context = getApplicationContext();
    auto workThreadName = QT_STRINGIFY(MC_QUICKBOOT_WORK_THREAD_NAME);
    if (context->containsBean(workThreadName)) {
        d->workThread = context->getBean<QThread *>(workThreadName);
    }
    if (d->workThread != nullptr && !d->workThread->isRunning()) {
        d->workThread->start();
    }
    for (auto &preloadBean : qAsConst(preloadBeans)) {
        context->getBean(preloadBean, d->workThread);
    }
}
