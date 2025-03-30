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
#include "McProperty.h"

#include <QXmlStreamWriter>

#include "McIocConstantGlobal.h"
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
