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
#include "McBeanCollection.h"

#include <QXmlStreamWriter>

#include "../IMcBean.h"
#include "McIocConstantGlobal.h"

namespace Mc::XmlBuilder {
MC_DECL_PRIVATE_DATA(BeanCollection)
QList<IBeanPtr> beans;
MC_DECL_PRIVATE_DATA_END

BeanCollection::BeanCollection() noexcept
{
    MC_NEW_PRIVATE_DATA(BeanCollection);
}

BeanCollection::~BeanCollection() {}

void BeanCollection::addBean(const IBeanPtr &bean) noexcept
{
    d->beans.append(bean);
}

void BeanCollection::addBeans(const QList<IBeanPtr> &beans) noexcept
{
    d->beans.append(beans);
}

void BeanCollection::writeToDevice(QIODevice *dev, int indent) const noexcept
{
    QXmlStreamWriter writer(dev);
    if (indent < 0) {
        writer.setAutoFormatting(false);
    } else {
        writer.setAutoFormatting(true);
        writer.setAutoFormattingIndent(indent);
    }
    writer.writeStartDocument();
    writer.writeStartElement(Mc::Constant::Tag::Xml::BEANS);
    for (auto &bean : qAsConst(d->beans)) {
        bean->write(writer);
    }
    writer.writeEndElement();
    writer.writeEndDocument();
}
} // namespace Mc::XmlBuilder
