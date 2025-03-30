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

#include <McCore/Callback/Impl/McAbstractSyncCallback.h>

#include "../McQuickBootMacroGlobal.h"

class QJSValue;
class QJSEngine;

MC_FORWARD_DECL_PRIVATE_DATA(McQmlSyncCallback);

class MC_QUICKBOOT_EXPORT McQmlSyncCallback : public McAbstractSyncCallback
{
    Q_OBJECT
    MC_DECL_INIT(McQmlSyncCallback)
    MC_INTERFACES(McAbstractSyncCallback)
    MC_DECL_POINTER(McQmlSyncCallback)
public:
    McQmlSyncCallback();
    McQmlSyncCallback(const QJSValue &callback, QJSEngine *engine) noexcept;
    ~McQmlSyncCallback() override;

    McQmlSyncCallback(const McQmlSyncCallback &o) noexcept;
    McQmlSyncCallback &operator=(const McQmlSyncCallback &o) noexcept;
    McQmlSyncCallback(McQmlSyncCallback &&o) noexcept;
    McQmlSyncCallback &operator=(McQmlSyncCallback &&o) noexcept;

    static McQmlSyncCallbackPtr build(const QJSValue &callback, QJSEngine *engine) noexcept
    {
        return McQmlSyncCallbackPtr(new McQmlSyncCallback(callback, engine), CallbackDeleter());
    }

protected:
    void syncCall(const QVariantList &varList) const noexcept override;

private:
    MC_DECL_COPYABLE_PRIVATE(McQmlSyncCallback)
};

#ifdef MC_USE_QT5
Q_DECLARE_METATYPE(McQmlSyncCallback)
#endif
MC_DECL_METATYPE(McQmlSyncCallback)
