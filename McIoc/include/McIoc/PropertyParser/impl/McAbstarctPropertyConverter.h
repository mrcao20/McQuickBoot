#pragma once

#include "../IMcPropertyConverter.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstarctPropertyConverter);

class McAbstarctPropertyConverter
        : public QObject
        , public IMcPropertyConverter 
{
    Q_OBJECT
public:
    explicit McAbstarctPropertyConverter(QObject *parent = nullptr);
    ~McAbstarctPropertyConverter() override;
    
    QVariant convert(IMcBeanReferenceResolver *resolver,
                     const QVariant &value) const noexcept override;
    
protected:
    IMcBeanReferenceResolver *resolver() const noexcept;
    
    virtual QVariant convertRef(const QVariant &value) const noexcept = 0;
    virtual QVariant convertEnum(const QVariant &value) const noexcept = 0;
    virtual QVariant convertList(const QVariant &value) const noexcept = 0;
    virtual QVariant convertMap(const QVariant &value) const noexcept = 0;
    
private:
    MC_DECL_PRIVATE(McAbstarctPropertyConverter)
};

MC_DECL_POINTER(McAbstarctPropertyConverter)
