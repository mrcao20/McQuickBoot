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
#include "McEnum.h"

#include <QXmlStreamWriter>

#include "McIocConstantGlobal.h"

namespace Mc::XmlBuilder {
MC_DECL_PRIVATE_DATA(Enum)
QString scope;
QString type;
QString value;
MC_DECL_PRIVATE_DATA_END

Enum::Enum() noexcept
{
    MC_NEW_PRIVATE_DATA(Enum);
}

Enum::~Enum() {}

void Enum::setScope(const QString &val) noexcept
{
    d->scope = val;
}

void Enum::setType(const QString &val) noexcept
{
    d->type = val;
}

void Enum::setValue(const QString &val) noexcept
{
    d->value = val;
}

void Enum::write(QXmlStreamWriter &writer) const noexcept
{
    writer.writeStartElement(Mc::Constant::Tag::Xml::ENUM);
    writer.writeTextElement(Mc::Constant::Tag::Xml::SCOPE, d->scope);
    writer.writeTextElement(Mc::Constant::Tag::Xml::TYPE, d->type);
    writer.writeTextElement(Mc::Constant::Tag::Xml::VALUE, d->value);
    writer.writeEndElement();
}
} // namespace Mc::XmlBuilder
