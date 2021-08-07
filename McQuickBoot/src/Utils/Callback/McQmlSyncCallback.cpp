/*
 * MIT License
 *
 * Copyright (c) 2021 mrcao20
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "McBoot/Utils/Callback/Impl/McQmlSyncCallback.h"

#include <QJSEngine>

#include "McBoot/Utils/McJsonUtils.h"

MC_STATIC()
qRegisterMetaType<McQmlSyncCallback>();
MC_REGISTER_BEAN_FACTORY(McQmlSyncCallback);
MC_STATIC_END

MC_DECL_PRIVATE_DATA(McQmlSyncCallback)
QJSValue callback;
QJSEngine *engine;
MC_DECL_PRIVATE_DATA_END

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

McQmlSyncCallback::McQmlSyncCallback(const McQmlSyncCallback &o) noexcept : McQmlSyncCallback()
{
    d->callback = o.d->callback;
    d->engine = o.d->engine;
}

McQmlSyncCallback::McQmlSyncCallback(McQmlSyncCallback &&o) noexcept
{
    d.swap(o.d);
}

McQmlSyncCallback &McQmlSyncCallback::operator=(const McQmlSyncCallback &o) noexcept
{
    d->callback = o.d->callback;
    d->engine = o.d->engine;
    return *this;
}

McQmlSyncCallback &McQmlSyncCallback::operator=(McQmlSyncCallback &&o) noexcept
{
    d.swap(o.d);
    return *this;
}

void McQmlSyncCallback::syncCall(const QVariantList &varList) noexcept
{
    QJSValueList args;
    for (auto param : varList) {
        param = McJsonUtils::serialize(param);
        args << d->engine->toScriptValue(param);
    }
    d->callback.call(args);
}
