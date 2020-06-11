#include "McIoc/PropertyParser/impl/McAbstarctPropertyConverter.h"

#include "McIoc/BeanFactory/impl/McBeanReference.h"

MC_DECL_PRIVATE_DATA(McAbstarctPropertyConverter)
IMcBeanReferenceResolver *resolver;
MC_DECL_PRIVATE_DATA_END

McAbstarctPropertyConverter::McAbstarctPropertyConverter(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstarctPropertyConverter)
}

McAbstarctPropertyConverter::~McAbstarctPropertyConverter() 
{
}

QVariant McAbstarctPropertyConverter::convert(IMcBeanReferenceResolver *resolver
                 , const QVariant &value) const noexcept 
{
    d->resolver = resolver;
    if(value.canConvert<McBeanReferencePtr>()) {
        return convertRef(value);
    }else if(value.canConvert<QVariantList>()) {
        return convertList(value);
    }else if(value.canConvert<QMap<QVariant, QVariant>>()) {
        return convertMap(value);
    }else{
        return value;
    }
}

IMcBeanReferenceResolver *McAbstarctPropertyConverter::resolver() const noexcept 
{
    return d->resolver;
}
