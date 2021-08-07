/*
 * MIT License
 *
 * Copyright (c) 2021 mrcao20
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
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
