/*
 * Copyright (c) 2021 mrcao20/mrcao20@163.com
 * McQuickBoot is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#include "McBean.h"

#include <QXmlStreamWriter>

#include "McIocConstantGlobal.h"
#include "Utils/XmlBuilder/IMcBeanContent.h"

namespace Mc::XmlBuilder {
MC_DECL_PRIVATE_DATA(Bean)
QString beanName;
QString isSingleton;
QString isPointer;
QString parent;
QString flag;
QVector<IBeanContentPtr> contents;
MC_DECL_PRIVATE_DATA_END

Bean::Bean() noexcept
{
    MC_NEW_PRIVATE_DATA(Bean);
}

Bean::Bean(const QString &beanName) noexcept
    : Bean()
{
    setBeanName(beanName);
}

Bean::~Bean() {}

void Bean::setBeanName(const QString &name) noexcept
{
    d->beanName = name;
}

void Bean::setSingleton(bool val) noexcept
{
    if (val) {
        d->isSingleton = "true";
    } else {
        d->isSingleton = "false";
    }
}

void Bean::setPointer(bool val) noexcept
{
    if (val) {
        d->isPointer = "true";
    } else {
        d->isPointer = "false";
    }
}

void Bean::setParent(const QString &name) noexcept
{
    d->parent = name;
}

void Bean::setFlag(const QString &val) noexcept
{
    d->flag = val;
}

void Bean::addContent(const IBeanContentPtr &content) noexcept
{
    d->contents.append(content);
}

void Bean::write(QXmlStreamWriter &writer) const noexcept
{
    writer.writeStartElement(Mc::Constant::Tag::Xml::BEAN);
    if (!d->beanName.isEmpty()) {
        writer.writeAttribute(Mc::Constant::Tag::Xml::NAME, d->beanName);
    }
    if (!d->isSingleton.isEmpty()) {
        writer.writeAttribute(Mc::Constant::Tag::Xml::IS_SINGLETON, d->isSingleton);
    }
    if (!d->isPointer.isEmpty()) {
        writer.writeAttribute(Mc::Constant::Tag::Xml::IS_POINTER, d->isPointer);
    }
    if (!d->parent.isEmpty()) {
        writer.writeAttribute(Mc::Constant::Tag::Xml::PARENT, d->parent);
    }
    if (!d->flag.isEmpty()) {
        writer.writeAttribute(Mc::Constant::Tag::Xml::FLAG, d->flag);
    }
    completeAttribute(writer);
    for (auto &content : qAsConst(d->contents)) {
        content->write(writer);
    }
    writer.writeEndElement();
}
} // namespace Mc::XmlBuilder
