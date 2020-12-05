#include  "McIoc/Utils/XmlBuilder/impl/McEnum.h"

namespace McXmlBuilder {

MC_DECL_PRIVATE_DATA(McEnum)
QString scope;
QString type;
QString value;
MC_DECL_PRIVATE_DATA_END

McEnum::McEnum() noexcept
{
    MC_NEW_PRIVATE_DATA(McEnum);
}

McEnum::~McEnum()
{
}

void McEnum::setScope(const QString &val) noexcept
{
    d->scope = val;
}

void McEnum::setType(const QString &val) noexcept
{
    d->type = val;
}

void McEnum::setValue(const QString &val) noexcept
{
    d->value = val;
}

QDomElement McEnum::toDomElement(QDomDocument &doc) const noexcept
{
    QDomElement e = doc.createElement("enum");
    e.setAttribute("scope", d->scope);
    e.setAttribute("type", d->type);
    e.setAttribute("value", d->value);
    return e;
}

}
