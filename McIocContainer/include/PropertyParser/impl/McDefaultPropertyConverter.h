#pragma once

#include "McAbstarctPropertyConverter.h"

MC_BEGIN_NAMESPACE

class MCIOCCONTAINER_EXPORT McDefaultPropertyConverter
        : public McAbstarctPropertyConverter {
    
    Q_OBJECT
public:
    explicit McDefaultPropertyConverter(QObject *parent = nullptr);
    
protected:
    QVariant convertRef(const QVariant &value) const noexcept override;
    QVariant convertList(const QVariant &value) const noexcept override;
    QVariant convertMap(const QVariant &value) const noexcept override;
};

MC_END_NAMESPACE

MC_FORWARD_DECL_CLASS(McDefaultPropertyConverter)
