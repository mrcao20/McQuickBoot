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
#include "McMap.h"

#include <QXmlStreamWriter>

#include "McIocConstantGlobal.h"
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

void Map::addContent(const IPropertyContentPtr &key, const QString &value) noexcept
{
    addContent(key, ValuePtr::create(value));
}

void Map::addContent(const QString &key, const QString &value) noexcept
{
    addContent(ValuePtr::create(key), ValuePtr::create(value));
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
