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
#include "McBean.h"

#include <QXmlStreamWriter>

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
