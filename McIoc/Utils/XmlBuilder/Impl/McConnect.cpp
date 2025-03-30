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
#include "McConnect.h"

#include <QXmlStreamWriter>

#include "McIocConstantGlobal.h"

namespace Mc::XmlBuilder {
MC_DECL_PRIVATE_DATA(Connect)
QString sender;
QString signal;
QString receiver;
QString slot;
QString connectionType;
MC_DECL_PRIVATE_DATA_END

Connect::Connect() noexcept
{
    MC_NEW_PRIVATE_DATA(Connect);
}

Connect::~Connect() {}

void Connect::setSender(const QString &val) noexcept
{
    d->sender = val;
}

void Connect::setSignal(const QString &val) noexcept
{
    d->signal = val;
}

void Connect::setReceiver(const QString &val) noexcept
{
    d->receiver = val;
}

void Connect::setSlot(const QString &val) noexcept
{
    d->slot = val;
}

void Connect::setConnectionType(const QString &val) noexcept
{
    d->connectionType = val;
}

void Connect::write(QXmlStreamWriter &writer) const noexcept
{
    writer.writeStartElement(Mc::Constant::Tag::Xml::CONNECT);
    writer.writeTextElement(Mc::Constant::Tag::Xml::SENDER, d->sender);
    writer.writeTextElement(Mc::Constant::Tag::Xml::SIGNAL_TAG, d->signal);
    writer.writeTextElement(Mc::Constant::Tag::Xml::RECEIVER, d->receiver);
    writer.writeTextElement(Mc::Constant::Tag::Xml::SLOT_TAG, d->slot);
    writer.writeTextElement(Mc::Constant::Tag::Xml::TYPE, d->connectionType);
    writer.writeEndElement();
}
} // namespace Mc::XmlBuilder
