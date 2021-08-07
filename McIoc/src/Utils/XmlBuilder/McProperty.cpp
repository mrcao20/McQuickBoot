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
#include "McIoc/Utils/XmlBuilder/impl/McProperty.h"

#include "McIoc/Utils/XmlBuilder/impl/McValue.h"

namespace McXmlBuilder {

MC_DECL_PRIVATE_DATA(McProperty)
QString name;
IMcPropertyContentPtr content;
MC_DECL_PRIVATE_DATA_END

McProperty::McProperty() noexcept
{
    MC_NEW_PRIVATE_DATA(McProperty);
}

McProperty::~McProperty()
{
}

void McProperty::setContent(const QString &name, IMcPropertyContentConstPtrRef val) noexcept
{
    d->name = name;
    d->content = val;
}

void McProperty::setContent(const QString &name, const QString &value) noexcept
{
    McValuePtr v = McValuePtr::create();
    v->setContent(value);
    setContent(name, v);
}

QDomElement McProperty::toDomElement(QDomDocument &doc) const noexcept
{
    QDomElement p = doc.createElement(Mc::Constant::Tag::Xml::property);
    p.setAttribute("name", d->name);
    p.appendChild(d->content->toDomElement(doc));
    return p;
}

}

