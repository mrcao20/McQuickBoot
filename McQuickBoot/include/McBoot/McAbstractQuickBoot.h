#pragma once

#include "IMcQuickBoot.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractQuickBoot);

class MCQUICKBOOT_EXPORT McAbstractQuickBoot : public QObject, public IMcQuickBoot
{
    Q_OBJECT
    MC_DECL_INIT(McAbstractQuickBoot)
public:
    using QObject::connect;

    explicit McAbstractQuickBoot(QObject *parent = nullptr);
    ~McAbstractQuickBoot() override;

private:
    MC_DECL_PRIVATE(McAbstractQuickBoot)
};

MC_DECL_POINTER(McAbstractQuickBoot);
