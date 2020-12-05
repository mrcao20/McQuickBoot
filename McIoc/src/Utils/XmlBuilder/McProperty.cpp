#include "McIoc/Utils/XmlBuilder/impl/McProperty.h"

#include "McIoc/Utils/XmlBuilder/impl/McValue.h"

namespace McXmlBuilder {

MC_DECL_PRIVATE_DATA(McProperty)
QString name;
IMcPropertyContentPtr content;
MC_DECL_PRIVATE_DATA_END

McProperty::McProperty() noexcept
{
    MC_NEW_PRIVATE_DATA(McProperty);
}

McProperty::~McProperty()
{
}

void McProperty::setContent(const QString &name, IMcPropertyContentConstPtrRef val) noexcept
{
    d->name = name;
    d->content = val;
}

void McProperty::setContent(const QString &name, const QString &value) noexcept
{
    McValuePtr v = McValuePtr::create();
    v->setContent(value);
    setContent(name, v);
}

QDomElement McProperty::toDomElement(QDomDocument &doc) const noexcept
{
    QDomElement p = doc.createElement(MC_PROPERTY_TAG);
    p.setAttribute("name", d->name);
    p.appendChild(d->content->toDomElement(doc));
    return p;
}

}

