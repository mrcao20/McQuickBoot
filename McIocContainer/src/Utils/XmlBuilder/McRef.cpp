#include "McIoc/Utils/XmlBuilder/impl/McRef.h"

namespace McXmlBuilder {

MC_DECL_PRIVATE_DATA(McRef)
QString beanName;
MC_DECL_PRIVATE_DATA_END

McRef::McRef() noexcept
{
    MC_NEW_PRIVATE_DATA(McRef);
}

McRef::~McRef()
{
}

void McRef::setBeanName(const QString &val) noexcept
{
    d->beanName = val;
}

QDomElement McRef::toDomElement(QDomDocument &doc) const noexcept
{
    QDomElement r = doc.createElement("ref");
    r.setAttribute("bean", d->beanName);
    return r;
}

}
