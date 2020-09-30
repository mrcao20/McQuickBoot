#include "McIoc/Utils/XmlBuilder/impl/McBeanCollection.h"

#include <QDomProcessingInstruction>
#include <QTextStream>
#include <QVector>

#include "McIoc/Utils/XmlBuilder/IMcBean.h"

namespace McXmlBuilder {

MC_DECL_PRIVATE_DATA(McBeanCollection)
QVector<IMcBeanPtr> beans;
MC_DECL_PRIVATE_DATA_END

McBeanCollection::McBeanCollection() noexcept
{
    MC_NEW_PRIVATE_DATA(McBeanCollection);
}

McBeanCollection::~McBeanCollection()
{
}

void McBeanCollection::addBean(IMcBeanConstPtrRef bean) noexcept
{
    d->beans.append(bean);
}

QDomDocument McBeanCollection::toDomDocument() const noexcept
{
    QDomDocument doc;
    auto xmlInstruction = doc.createProcessingInstruction("xml", R"(version="1.0" encoding="UTF-8")");
    doc.appendChild(xmlInstruction);
    QDomElement root = doc.createElement(MC_BEANS_TAG);
    doc.appendChild(root);
    for(auto bean : d->beans) {
        root.appendChild(bean->toDomElement(doc));
    }
    return doc;
}

void McBeanCollection::writeToDevice(QIODevice *dev, int indent) const noexcept
{
    auto doc = toDomDocument();
    QTextStream stream(dev);
    doc.save(stream, indent);
}

}
