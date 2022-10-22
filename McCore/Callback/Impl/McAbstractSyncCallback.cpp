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
#include "McAbstractSyncCallback.h"

#include <QCoreApplication>

void McAbstractSyncCallback::destroy() noexcept
{
    deleteLater();
}

QVariant McAbstractSyncCallback::call(const QVariantList &varList) const noexcept
{
    QCoreApplication::postEvent(
        const_cast<McAbstractSyncCallback *>(this), new McCustomEvent(QEvent::Type::User, varList));
    return QVariant();
}

void McAbstractSyncCallback::customEvent(QEvent *event)
{
    if (event->type() == QEvent::Type::User) {
        McCustomEvent *e = static_cast<McCustomEvent *>(event);
        syncCall(e->data().toList());
    }
}
