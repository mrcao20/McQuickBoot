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
#include "McIoc/Utils/XmlBuilder/impl/McConnect.h"

namespace McXmlBuilder {

MC_DECL_PRIVATE_DATA(McConnect)
QString sender;
QString signal;
QString receiver;
QString slot;
QString connectionType;
MC_DECL_PRIVATE_DATA_END

McConnect::McConnect() noexcept
{
    MC_NEW_PRIVATE_DATA(McConnect);
}

McConnect::~McConnect()
{
}

void McConnect::setSender(const QString &val) noexcept
{
    d->sender = val;
}

void McConnect::setSignal(const QString &val) noexcept
{
    d->signal = val;
}

void McConnect::setReceiver(const QString &val) noexcept
{
    d->receiver = val;
}

void McConnect::setSlot(const QString &val) noexcept
{
    d->slot = val;
}

void McConnect::setConnectionType(const QString &val) noexcept
{
    d->connectionType = val;
}

QDomElement McConnect::toDomElement(QDomDocument &doc) const noexcept
{
    QDomElement con = doc.createElement(Mc::Constant::Tag::Xml::connect);
    con.setAttribute(Mc::Constant::Tag::Xml::sender, d->sender);
    con.setAttribute(Mc::Constant::Tag::Xml::signal, d->signal);
    con.setAttribute(Mc::Constant::Tag::Xml::receiver, d->receiver);
    con.setAttribute(Mc::Constant::Tag::Xml::slot, d->slot);
    con.setAttribute(Mc::Constant::Tag::Xml::type, d->connectionType);
    return con;
}

}
