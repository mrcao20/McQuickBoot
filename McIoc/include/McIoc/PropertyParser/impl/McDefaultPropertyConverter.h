#pragma once

#include "McAbstarctPropertyConverter.h"

class MCIOC_EXPORT McDefaultPropertyConverter
        : public McAbstarctPropertyConverter 
{
    Q_OBJECT
public:
    explicit McDefaultPropertyConverter(QObject *parent = nullptr);
    
protected:
    QVariant convertRef(const QVariant &value) const noexcept override;
    QVariant convertEnum(const QVariant &value) const noexcept override;
    QVariant convertList(const QVariant &value) const noexcept override;
    QVariant convertMap(const QVariant &value) const noexcept override;
};

MC_DECL_POINTER(McDefaultPropertyConverter)
