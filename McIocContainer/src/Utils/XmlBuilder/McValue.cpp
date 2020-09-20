#include "McIoc/Utils/XmlBuilder/impl/McValue.h"

#include <QDomText>

namespace McXmlBuilder {

MC_DECL_PRIVATE_DATA(McValue)
QString content;
MC_DECL_PRIVATE_DATA_END

McValue::McValue() noexcept
{
    MC_NEW_PRIVATE_DATA(McValue);
}

McValue::~McValue()
{
}

void McValue::setContent(const QString &val) noexcept
{
    d->content = val;
}

QDomElement McValue::toDomElement(QDomDocument &doc) const noexcept
{
    QDomElement v = doc.createElement("value");
    v.appendChild(doc.createTextNode(d->content));
    return v;
}

}
