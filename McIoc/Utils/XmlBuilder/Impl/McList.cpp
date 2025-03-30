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
#include "McList.h"

#include <QXmlStreamWriter>

#include "McIocConstantGlobal.h"
#include "McValue.h"

namespace Mc::XmlBuilder {
MC_DECL_PRIVATE_DATA(List)
QVector<IPropertyContentPtr> contents;
MC_DECL_PRIVATE_DATA_END

List::List() noexcept
{
    MC_NEW_PRIVATE_DATA(List);
}

List::~List() {}

void List::addContent(const IPropertyContentPtr &val) noexcept
{
    d->contents.append(val);
}

void List::addContent(const QString &val) noexcept
{
    ValuePtr v = ValuePtr::create();
    v->setContent(val);
    addContent(v);
}

void List::write(QXmlStreamWriter &writer) const noexcept
{
    writer.writeStartElement(Mc::Constant::Tag::Xml::LIST);
    for (auto &content : qAsConst(d->contents)) {
        content->write(writer);
    }
    writer.writeEndElement();
}
} // namespace Mc::XmlBuilder
