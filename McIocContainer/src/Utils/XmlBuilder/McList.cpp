#include "McIoc/Utils/XmlBuilder/impl/McList.h"

namespace McXmlBuilder {

MC_DECL_PRIVATE_DATA(McList)
QVector<IMcPropertyContentPtr> contents;
MC_DECL_PRIVATE_DATA_END

McList::McList() noexcept
{
    MC_NEW_PRIVATE_DATA(McList);
}

McList::~McList()
{
}

void McList::addContent(IMcPropertyContentConstPtrRef val) noexcept
{
    d->contents.append(val);
}

QDomElement McList::toDomElement(QDomDocument &doc) const noexcept
{
    QDomElement l = doc.createElement("list");
    for(auto content : d->contents) {
        l.appendChild(content->toDomElement(doc));
    }
    return l;
}

}
