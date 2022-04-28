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
#include "McBeanCollection.h"

#include <QXmlStreamWriter>

#include "../IMcBean.h"

namespace Mc::XmlBuilder {
MC_DECL_PRIVATE_DATA(BeanCollection)
QVector<IBeanPtr> beans;
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
