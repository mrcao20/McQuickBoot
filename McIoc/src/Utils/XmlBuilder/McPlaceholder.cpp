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
#include "McIoc/Utils/XmlBuilder/impl/McPlaceholder.h"

namespace McXmlBuilder {

MC_DECL_PRIVATE_DATA(McPlaceholder)
QString placeholder;
MC_DECL_PRIVATE_DATA_END

McPlaceholder::McPlaceholder() noexcept
{
    MC_NEW_PRIVATE_DATA(McPlaceholder);
}

McPlaceholder::~McPlaceholder() {}

void McPlaceholder::setPlaceholder(const QString &val) noexcept
{
    d->placeholder = val;
}

QDomElement McPlaceholder::toDomElement(QDomDocument &doc) const noexcept
{
    QDomElement v = doc.createElement("plh");
    v.appendChild(doc.createTextNode(d->placeholder));
    return v;
}

} // namespace McXmlBuilder
