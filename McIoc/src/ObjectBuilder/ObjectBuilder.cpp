#include "McIoc/ObjectBuilder/ObjectBuilder.h"

#include <QMap>

namespace McPrivate {

using QObjectBuilderType = QMap<int, IQObjectBuilderPtr>;
MC_GLOBAL_STATIC(QObjectBuilderType, qobjectBuilders)

void IQObjectBuilder::addQObjectBuilder(int id, const IQObjectBuilderPtr &builder) noexcept
{
    qobjectBuilders->insert(id, builder);
}

IQObjectBuilderPtr IQObjectBuilder::getQObjectBuilder(int id) noexcept
{
    return qobjectBuilders->value(id);
}

} // namespace McPrivate
