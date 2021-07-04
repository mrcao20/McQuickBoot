#pragma once

#include "../McBootGlobal.h"

class IMcModelContainer
{
public:
    virtual ~IMcModelContainer() = default;

    virtual QObject *getModel(const QString &beanName) noexcept = 0;
};

MC_DECL_METATYPE(IMcModelContainer)
