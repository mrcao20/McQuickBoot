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
#include "McValue.h"

#include <QXmlStreamWriter>

#include "McIocConstantGlobal.h"

namespace Mc::XmlBuilder {
MC_DECL_PRIVATE_DATA(Value)
QString content;
MC_DECL_PRIVATE_DATA_END

Value::Value() noexcept
{
    MC_NEW_PRIVATE_DATA(Value);
}

Value::Value(const QString &val) noexcept
    : Value()
{
    d->content = val;
}

Value::~Value() {}

void Value::setContent(const QString &val) noexcept
{
    d->content = val;
}

void Value::write(QXmlStreamWriter &writer) const noexcept
{
    writer.writeTextElement(Mc::Constant::Tag::Xml::VALUE, d->content);
}
} // namespace Mc::XmlBuilder
