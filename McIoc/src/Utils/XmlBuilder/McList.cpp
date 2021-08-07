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
#include "McIoc/Utils/XmlBuilder/impl/McList.h"

#include <QVector>

#include "McIoc/Utils/XmlBuilder/impl/McValue.h"

namespace McXmlBuilder {

MC_DECL_PRIVATE_DATA(McList)
QVector<IMcPropertyContentPtr> contents;
MC_DECL_PRIVATE_DATA_END

McList::McList() noexcept
{
    MC_NEW_PRIVATE_DATA(McList);
}

McList::~McList()
{
}

void McList::addContent(IMcPropertyContentConstPtrRef val) noexcept
{
    d->contents.append(val);
}

void McList::addContent(const QString &val) noexcept
{
    McValuePtr v = McValuePtr::create();
    v->setContent(val);
    addContent(v);
}

QDomElement McList::toDomElement(QDomDocument &doc) const noexcept
{
    QDomElement l = doc.createElement("list");
    for(auto content : d->contents) {
        l.appendChild(content->toDomElement(doc));
    }
    return l;
}

}
