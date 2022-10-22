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

#include "../IMcCallback.h"

class MC_CORE_EXPORT McAbstractSyncCallback
    : public QObject
    , public IMcCallback
{
    Q_OBJECT
    MC_INTERFACES(IMcCallback)
public:
    using IMcCallback::call;

    void destroy() noexcept override;
    QVariant call(const QVariantList &varList) const noexcept override final;

protected:
    virtual void syncCall(const QVariantList &varList) const noexcept = 0;

    void customEvent(QEvent *event) override;
};

MC_DECL_METATYPE(McAbstractSyncCallback)
