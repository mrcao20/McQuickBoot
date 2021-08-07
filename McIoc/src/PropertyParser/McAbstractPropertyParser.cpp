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
#include "McIoc/PropertyParser/impl/McAbstractPropertyParser.h"

#include <QDomElement>
#include <QDebug>

McAbstractPropertyParser::McAbstractPropertyParser(QObject *parent)
    : QObject(parent)
{
}

QVariant McAbstractPropertyParser::parse(const QDomElement &ele) const noexcept
{
    QDomElement childEle;
    if ((childEle = ele).tagName() == "list"
        || !(childEle = ele.firstChildElement("list")).isNull()) {
        return parseList(childEle);
    } else if ((childEle = ele).tagName() == "map"
               || !(childEle = ele.firstChildElement("map")).isNull()) {
        return parseMap(childEle);
    } else if ((childEle = ele).tagName() == "enum"
               || !(childEle = ele.firstChildElement("enum")).isNull()) {
        return parseEnum(childEle);
    } else if ((childEle = ele).tagName() == "value" || (childEle = ele).hasAttribute("value")
               || !(childEle = ele.firstChildElement("value")).isNull()) {
        return parseValue(childEle);
    } else if ((childEle = ele).tagName() == "ref" || (childEle = ele).hasAttribute("ref")
               || !(childEle = ele.firstChildElement("ref")).isNull()) {
        return parseRef(childEle);
    } else {
        qWarning("the tag for '%s' cannot parse!!\n", qPrintable(ele.tagName()));
        return QVariant();
    }
}
