#pragma once

#include "../McGlobal.h"

#include <QVariant>

class IMcCustomPlaceholder
{
public:
    virtual ~IMcCustomPlaceholder() = default;

    virtual QVariant getKey() const noexcept = 0;
};

MC_DECL_METATYPE(IMcCustomPlaceholder)
