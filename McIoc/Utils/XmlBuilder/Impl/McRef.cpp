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
#include "McRef.h"

#include <QXmlStreamWriter>

#include "McIocConstantGlobal.h"

namespace Mc::XmlBuilder {
MC_DECL_PRIVATE_DATA(Ref)
QString beanName;
MC_DECL_PRIVATE_DATA_END

Ref::Ref() noexcept
{
    MC_NEW_PRIVATE_DATA(Ref);
}

Ref::Ref(const QString &val) noexcept
    : Ref()
{
    d->beanName = val;
}

Ref::~Ref() {}

void Ref::setBeanName(const QString &val) noexcept
{
    d->beanName = val;
}

void Ref::write(QXmlStreamWriter &writer) const noexcept
{
    writer.writeTextElement(Mc::Constant::Tag::Xml::REF, d->beanName);
}
} // namespace Mc::XmlBuilder
