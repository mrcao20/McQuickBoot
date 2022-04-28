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
#include "McConstructorArg.h"

#include <QXmlStreamWriter>

#include "McValue.h"

namespace Mc::XmlBuilder {
MC_DECL_PRIVATE_DATA(ConstructorArg)
int index{-1};
QString name;
IPropertyContentPtr value;
MC_DECL_PRIVATE_DATA_END

ConstructorArg::ConstructorArg() noexcept
{
    MC_NEW_PRIVATE_DATA(ConstructorArg);
}

ConstructorArg::ConstructorArg(int index, const IPropertyContentPtr &val) noexcept
    : ConstructorArg()
{
    setIndex(index);
    setContent(val);
}

ConstructorArg::ConstructorArg(int index, const QString &value) noexcept
    : ConstructorArg()
{
    setIndex(index);
    setContent(value);
}

ConstructorArg::ConstructorArg(const QString &name, const IPropertyContentPtr &val) noexcept
    : ConstructorArg()
{
    setName(name);
    setContent(val);
}

ConstructorArg::ConstructorArg(const QString &name, const QString &value) noexcept
    : ConstructorArg()
{
    setName(name);
    setContent(value);
}

ConstructorArg::~ConstructorArg() {}

void ConstructorArg::setIndex(int val) noexcept
{
    d->index = val;
}

void ConstructorArg::setName(const QString &name) noexcept
{
    d->name = name;
}

void ConstructorArg::setContent(const IPropertyContentPtr &val) noexcept
{
    d->value = val;
}

void ConstructorArg::setContent(const QString &value) noexcept
{
    setContent(ValuePtr::create(value));
}

void ConstructorArg::write(QXmlStreamWriter &writer) const noexcept
{
    if (d->value.isNull()) {
        return;
    }
    writer.writeStartElement(Mc::Constant::Tag::Xml::CONSTRUCTOR_ARG);
    if (d->index != -1) {
        writer.writeAttribute(Mc::Constant::Tag::Xml::INDEX, QString::number(d->index));
    } else if (!d->name.isEmpty()) {
        writer.writeAttribute(Mc::Constant::Tag::Xml::NAME, d->name);
    }
    d->value->write(writer);
    writer.writeEndElement();
}
} // namespace Mc::XmlBuilder
