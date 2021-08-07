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
#include "McIoc/Utils/XmlBuilder/impl/McEnum.h"

namespace McXmlBuilder {

MC_DECL_PRIVATE_DATA(McEnum)
QString scope;
QString type;
QString value;
MC_DECL_PRIVATE_DATA_END

McEnum::McEnum() noexcept
{
    MC_NEW_PRIVATE_DATA(McEnum);
}

McEnum::~McEnum()
{
}

void McEnum::setScope(const QString &val) noexcept
{
    d->scope = val;
}

void McEnum::setType(const QString &val) noexcept
{
    d->type = val;
}

void McEnum::setValue(const QString &val) noexcept
{
    d->value = val;
}

QDomElement McEnum::toDomElement(QDomDocument &doc) const noexcept
{
    QDomElement e = doc.createElement("enum");
    e.setAttribute("scope", d->scope);
    e.setAttribute("type", d->type);
    e.setAttribute("value", d->value);
    return e;
}

}
