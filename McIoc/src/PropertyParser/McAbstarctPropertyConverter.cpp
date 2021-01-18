#include "McIoc/PropertyParser/impl/McAbstarctPropertyConverter.h"

#include "McIoc/BeanFactory/impl/McBeanReference.h"
#include "McIoc/BeanFactory/impl/McBeanEnum.h"

MC_DECL_PRIVATE_DATA(McAbstarctPropertyConverter)
QList<IMcBeanReferenceResolver *> resolvers;
MC_DECL_PRIVATE_DATA_END

McAbstarctPropertyConverter::McAbstarctPropertyConverter(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstarctPropertyConverter)
}

McAbstarctPropertyConverter::~McAbstarctPropertyConverter() 
{
}

QVariant McAbstarctPropertyConverter::convert(const QVariant &value) const noexcept
{
    if(value.canConvert<McBeanReferencePtr>()) {
        return convertRef(value);
    } else if(value.canConvert<McBeanEnumPtr>()) {
        return convertEnum(value);
    } else if(value.canConvert<QVariantList>()) {
        return convertList(value);
    } else if(value.canConvert<QMap<QVariant, QVariant>>()) {
        return convertMap(value);
    } else{
        return value;
    }
}

void McAbstarctPropertyConverter::addReferenceResolver(IMcBeanReferenceResolver *resolver) noexcept
{
    d->resolvers.append(resolver);
}

QList<IMcBeanReferenceResolver *> McAbstarctPropertyConverter::resolvers() const noexcept
{
    return d->resolvers;
}
