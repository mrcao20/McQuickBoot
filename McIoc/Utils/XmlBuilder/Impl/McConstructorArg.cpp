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
#include "McConstructorArg.h"

#include <QXmlStreamWriter>

#include "McIocConstantGlobal.h"
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
