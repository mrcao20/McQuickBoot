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
#include "McQmlSyncCallback.h"

#include <QJSEngine>

#include <McCore/Utils/McJsonUtils.h>

MC_INIT2(McQmlSyncCallback)
{
    mcRegisterMetaTypeSimple<McQmlSyncCallback>();
}

MC_DECL_PRIVATE_DATA2(McQmlSyncCallback)
{
    QJSValue callback;
    QJSEngine *engine;
};

McQmlSyncCallback::McQmlSyncCallback()
{
    MC_NEW_PRIVATE_DATA(McQmlSyncCallback);
}

McQmlSyncCallback::McQmlSyncCallback(const QJSValue &callback, QJSEngine *engine) noexcept
    : McQmlSyncCallback()
{
    d->callback = callback;
    d->engine = engine;
}

McQmlSyncCallback::~McQmlSyncCallback() {}

McQmlSyncCallback::McQmlSyncCallback(const McQmlSyncCallback &o) noexcept
    : McQmlSyncCallback(o.d->callback, o.d->engine)
{
}

McQmlSyncCallback &McQmlSyncCallback::operator=(const McQmlSyncCallback &o) noexcept
{
    McQmlSyncCallback copy(o);
    d.swap(copy.d);
    return *this;
}

McQmlSyncCallback::McQmlSyncCallback(McQmlSyncCallback &&o) noexcept
{
    d.swap(o.d);
}

McQmlSyncCallback &McQmlSyncCallback::operator=(McQmlSyncCallback &&o) noexcept
{
    McQmlSyncCallback copy(std::move(o));
    d.swap(copy.d);
    return *this;
}

void McQmlSyncCallback::syncCall(const QVariantList &varList) const noexcept
{
    QJSValueList args;
    args.reserve(varList.size());
    for (auto param : varList) {
        param = McJsonUtils::serialize(param);
        args << d->engine->toScriptValue(param);
    }
    d->callback.call(args);
}
