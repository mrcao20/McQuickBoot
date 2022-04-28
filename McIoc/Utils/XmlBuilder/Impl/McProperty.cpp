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
#include "McProperty.h"

#include <QXmlStreamWriter>

#include "McValue.h"

namespace Mc::XmlBuilder {
MC_DECL_PRIVATE_DATA(Property)
QString name;
IPropertyContentPtr content;
MC_DECL_PRIVATE_DATA_END

Property::Property() noexcept
{
    MC_NEW_PRIVATE_DATA(Property);
}

Property::Property(const QString &name, const IPropertyContentPtr &val) noexcept
    : Property()
{
    setContent(name, val);
}

Property::Property(const QString &name, const QString &value) noexcept
    : Property(name, ValuePtr::create(value))
{
}

Property::~Property() {}

void Property::setContent(const QString &name, const IPropertyContentPtr &val) noexcept
{
    d->name = name;
    d->content = val;
}

void Property::setContent(const QString &name, const QString &value) noexcept
{
    ValuePtr v = ValuePtr::create();
    v->setContent(value);
    setContent(name, v);
}

void Property::write(QXmlStreamWriter &writer) const noexcept
{
    writer.writeStartElement(Mc::Constant::Tag::Xml::PROPERTY);
    writer.writeAttribute(Mc::Constant::Tag::Xml::NAME, d->name);
    d->content->write(writer);
    writer.writeEndElement();
}
} // namespace Mc::XmlBuilder
