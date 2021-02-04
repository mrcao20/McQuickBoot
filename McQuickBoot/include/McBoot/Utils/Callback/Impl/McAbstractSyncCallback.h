#pragma once

#include "../IMcCallback.h"
#include <QObject>

#include "../../../McBootMacroGlobal.h"

class MCQUICKBOOT_EXPORT McAbstractSyncCallback : public QObject, public IMcCallback
{
    Q_OBJECT
    MC_TYPELIST(IMcCallback)
public:
    using IMcCallback::call;

    void destroy() noexcept override;
    void call(const QVariantList &varList) noexcept override final;

protected:
    virtual void syncCall(const QVariantList &varList) noexcept = 0;

    void customEvent(QEvent *event) override;
};

MC_DECL_METATYPE(McAbstractSyncCallback)
