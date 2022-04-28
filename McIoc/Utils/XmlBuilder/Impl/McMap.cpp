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
#include "McMap.h"

#include <QXmlStreamWriter>

#include "McValue.h"

namespace Mc::XmlBuilder {
MC_DECL_PRIVATE_DATA(Map)
QMap<IPropertyContentPtr, IPropertyContentPtr> contents;
MC_DECL_PRIVATE_DATA_END

Map::Map() noexcept
{
    MC_NEW_PRIVATE_DATA(Map);
}

Map::~Map() {}

void Map::addContent(const IPropertyContentPtr &key, const IPropertyContentPtr &value) noexcept
{
    d->contents.insert(key, value);
}

void Map::addContent(const QString &key, const IPropertyContentPtr &value) noexcept
{
    auto v = ValuePtr::create();
    v->setContent(key);
    addContent(v, value);
}

void Map::write(QXmlStreamWriter &writer) const noexcept
{
    writer.writeStartElement(Mc::Constant::Tag::Xml::MAP);
    QMapIterator<IPropertyContentPtr, IPropertyContentPtr> itr(d->contents);
    while (itr.hasNext()) {
        auto item = itr.next();
        writer.writeStartElement(Mc::Constant::Tag::Xml::ENTRY);

        writer.writeStartElement(Mc::Constant::Tag::Xml::KEY);
        item.key()->write(writer);
        writer.writeEndElement();

        writer.writeStartElement(Mc::Constant::Tag::Xml::VALUE);
        item.value()->write(writer);
        writer.writeEndElement();

        writer.writeEndElement();
    }
    writer.writeEndElement();
}
} // namespace Mc::XmlBuilder
