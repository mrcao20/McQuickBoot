#include "McIoc/Utils/XmlBuilder/impl/McBean.h"

#include <QVector>

#include "McIoc/Utils/XmlBuilder/IMcBeanContent.h"

namespace McXmlBuilder {

MC_DECL_PRIVATE_DATA(McBean)
QString beanName;
QString className;
QString pluginPath;
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

void McBean::addContent(IMcBeanContentConstPtrRef content) noexcept
{
    d->contents.append(content);
}

QDomElement McBean::toDomElement(QDomDocument &doc) const noexcept
{
    QDomElement bean = doc.createElement(MC_BEAN_TAG);
    if(!d->beanName.isEmpty()) {
        bean.setAttribute("name", d->beanName);
    }
    if(!d->pluginPath.isEmpty()) {
        bean.setAttribute("plugin", d->pluginPath);
    } else {
        bean.setAttribute("class", d->className);
    }
    for(auto content : d->contents) {
        bean.appendChild(content->toDomElement(doc));
    }
    return bean;
}

}
