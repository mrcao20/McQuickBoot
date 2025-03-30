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
#include "McPlaceholder.h"

#include <QXmlStreamWriter>

#include "McIocConstantGlobal.h"

namespace Mc::XmlBuilder {
MC_DECL_PRIVATE_DATA(Placeholder)
QString placeholder;
MC_DECL_PRIVATE_DATA_END

Placeholder::Placeholder() noexcept
{
    MC_NEW_PRIVATE_DATA(Placeholder);
}

Placeholder::Placeholder(const QString &val) noexcept
    : Placeholder()
{
    d->placeholder = val;
}

Placeholder::~Placeholder() {}

void Placeholder::setPlaceholder(const QString &val) noexcept
{
    d->placeholder = val;
}

void Placeholder::write(QXmlStreamWriter &writer) const noexcept
{
    writer.writeTextElement(Mc::Constant::Tag::Xml::PLH, d->placeholder);
}
} // namespace Mc::XmlBuilder
