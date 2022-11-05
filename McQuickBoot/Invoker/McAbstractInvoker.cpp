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
#include "McAbstractInvoker.h"

#include <McIoc/ApplicationContext/IMcApplicationContext.h>

#include "McQuickBootGlobal.h"

MC_DECL_PRIVATE_DATA2(McAbstractInvoker)
{
    IMcApplicationContext *appCtx{nullptr};
};

McAbstractInvoker::McAbstractInvoker(QObject *parent) noexcept
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractInvoker);
}

McAbstractInvoker::~McAbstractInvoker() {}

IMcApplicationContext *McAbstractInvoker::appCtx() const noexcept
{
    return d->appCtx;
}

void McAbstractInvoker::setAppCtx(IMcApplicationContext *val) noexcept
{
    d->appCtx = val;
}

QObject *McAbstractInvoker::getBean(const QString &name) const noexcept
{
    auto var = getBeanToVariant(name);
    QObject *obj = var.value<QObject *>();
    if (obj == nullptr) {
        obj = var.value<QObjectPtr>().data();
    }
    if (obj == nullptr) {
        qCWarning(mcQuickBoot) << "cannot get bean for named:" << name;
    } else {
        //#ifndef MC_TINY_QUICK_BOOT
        //        QQmlEngine::setObjectOwnership(obj, QQmlEngine::CppOwnership);
        //#endif
    }
    return obj;
}

// QObject *McAbstractInvoker::getModel(const QString &name) const noexcept
//{
//     return d->modelContainer->getModel(name);
// }

QVariant McAbstractInvoker::getBeanToVariant(const QString &name) const noexcept
{
    return d->appCtx->getBean(name);
}
