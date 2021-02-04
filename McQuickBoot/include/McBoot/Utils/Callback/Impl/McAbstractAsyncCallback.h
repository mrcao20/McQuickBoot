#pragma once

#include "../IMcCallback.h"
#include <QObject>

#include "../../../McBootMacroGlobal.h"

class MCQUICKBOOT_EXPORT McAbstractAsyncCallback : public QObject, public IMcCallback
{
    Q_OBJECT
    MC_TYPELIST(IMcCallback)
public:
    void destroy() noexcept override;
};

MC_DECL_METATYPE(McAbstractAsyncCallback)
