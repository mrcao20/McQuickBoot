#include "McIoc/Utils/XmlBuilder/impl/McMap.h"

#include "McIoc/Utils/XmlBuilder/impl/McValue.h"

namespace McXmlBuilder {

MC_DECL_PRIVATE_DATA(McMap)
QMap<IMcPropertyContentPtr, IMcPropertyContentPtr> contents;
MC_DECL_PRIVATE_DATA_END

McMap::McMap() noexcept
{
    MC_NEW_PRIVATE_DATA(McMap);
}

McMap::~McMap()
{
}

void McMap::addContent(IMcPropertyContentConstPtrRef key
                       , IMcPropertyContentConstPtrRef value) noexcept
{
    d->contents.insert(key, value);
}

void McMap::addContent(const QString &key
                       , IMcPropertyContentConstPtrRef value) noexcept
{
    auto v = McValuePtr::create();
    v->setContent(key);
    addContent(v, value);
}

QDomElement McMap::toDomElement(QDomDocument &doc) const noexcept
{
    QDomElement m = doc.createElement("map");
    for(auto k : d->contents.keys()) {
        auto v = d->contents.value(k);
        QDomElement entry = doc.createElement("entry");
        
        QDomElement key = doc.createElement("key");
        key.appendChild(k->toDomElement(doc));
        QDomElement value = doc.createElement("value");
        value.appendChild(v->toDomElement(doc));
        
        entry.appendChild(key);
        entry.appendChild(value);
        m.appendChild(entry);
    }
    return m;
}

}
