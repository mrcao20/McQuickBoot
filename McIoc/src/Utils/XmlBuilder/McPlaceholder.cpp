#include "McIoc/Utils/XmlBuilder/impl/McPlaceholder.h"

namespace McXmlBuilder {

MC_DECL_PRIVATE_DATA(McPlaceholder)
QString placeholder;
MC_DECL_PRIVATE_DATA_END

McPlaceholder::McPlaceholder() noexcept
{
    MC_NEW_PRIVATE_DATA(McPlaceholder);
}

McPlaceholder::~McPlaceholder() {}

void McPlaceholder::setPlaceholder(const QString &val) noexcept
{
    d->placeholder = val;
}

QDomElement McPlaceholder::toDomElement(QDomDocument &doc) const noexcept
{
    QDomElement v = doc.createElement("plh");
    v.appendChild(doc.createTextNode(d->placeholder));
    return v;
}

} // namespace McXmlBuilder
