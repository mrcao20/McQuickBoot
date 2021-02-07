#include "McIoc/Utils/XmlBuilder/impl/McBean.h"

#include <QVector>

#include "McIoc/Utils/XmlBuilder/IMcBeanContent.h"

namespace McXmlBuilder {

MC_DECL_PRIVATE_DATA(McBean)
QString beanName;
QString className;
QString pluginPath;
QString isSingleton;
QString isPointer;
QVector<IMcBeanContentPtr> contents;
MC_DECL_PRIVATE_DATA_END

McBean::McBean() noexcept
{
    MC_NEW_PRIVATE_DATA(McBean);
}

McBean::~McBean()
{
}

void McBean::setBeanName(const QString &name) noexcept
{
    d->beanName = name;
}

void McBean::setClassName(const QString &name) noexcept
{
    d->className = name;
}

void McBean::setPluginPath(const QString &path) noexcept
{
    d->pluginPath = path;
}

void McBean::setSingleton(bool val) noexcept
{
    if (val) {
        d->isSingleton = "true";
    } else {
        d->isSingleton = "false";
    }
}

void McBean::setPointer(bool val) noexcept
{
    if (val) {
        d->isPointer = "true";
    } else {
        d->isPointer = "false";
    }
}

void McBean::addContent(IMcBeanContentConstPtrRef content) noexcept
{
    d->contents.append(content);
}

QDomElement McBean::toDomElement(QDomDocument &doc) const noexcept
{
    QDomElement bean = doc.createElement(Mc::Constant::Tag::Xml::bean);
    if(!d->beanName.isEmpty()) {
        bean.setAttribute("name", d->beanName);
    }
    if(!d->pluginPath.isEmpty()) {
        bean.setAttribute("plugin", d->pluginPath);
    } else {
        bean.setAttribute("class", d->className);
    }
    if (!d->isSingleton.isEmpty()) {
        bean.setAttribute("isSingleton", d->isSingleton);
    }
    if (!d->isPointer.isEmpty()) {
        bean.setAttribute("isPointer", d->isPointer);
    }
    for(auto content : d->contents) {
        bean.appendChild(content->toDomElement(doc));
    }
    return bean;
}

}
