#pragma once

#include "McAbstractSyncCallback.h"

#include <QJSValue>

class QJSEngine;

MC_FORWARD_DECL_PRIVATE_DATA(McQmlSyncCallback);

class McQmlSyncCallback : public McAbstractSyncCallback
{
    Q_OBJECT
    MC_TYPELIST(McAbstractSyncCallback)
public:
    McQmlSyncCallback();
    McQmlSyncCallback(const QJSValue &callback, QJSEngine *engine) noexcept;
    ~McQmlSyncCallback() override;

    McQmlSyncCallback(const McQmlSyncCallback &o) noexcept;
    McQmlSyncCallback(McQmlSyncCallback &&o) noexcept;
    void operator=(const McQmlSyncCallback &o) noexcept;
    void operator=(McQmlSyncCallback &&o) noexcept;

protected:
    void syncCall(const QVariantList &varList) noexcept override;

private:
    MC_DECL_PRIVATE(McQmlSyncCallback)
};

Q_DECLARE_METATYPE(McQmlSyncCallback)
MC_DECL_METATYPE(McQmlSyncCallback)
