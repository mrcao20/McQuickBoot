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
#include "McPluginChecker.h"

#include <QScopeGuard>
#include <QXmlStreamWriter>

#include "McIocConstantGlobal.h"

namespace Mc::XmlBuilder {
MC_DECL_PRIVATE_DATA(PluginChecker)
QString metaData;
QString className;
QString refName;
MC_DECL_PRIVATE_DATA_END

PluginChecker::PluginChecker() noexcept
{
    MC_NEW_PRIVATE_DATA(PluginChecker);
}

PluginChecker::~PluginChecker() {}

void PluginChecker::setMetaData(const QString &val) noexcept
{
    d->metaData = val;
}

void PluginChecker::setClassName(const QString &val) noexcept
{
    d->className = val;
}

void PluginChecker::setRefName(const QString &val) noexcept
{
    d->refName = val;
}

void PluginChecker::write(QXmlStreamWriter &writer) const noexcept
{
    writer.writeStartElement(Mc::Constant::Tag::Xml::PLUGIN);
    auto cleanup = qScopeGuard([&writer]() { writer.writeEndElement(); });

    if (!d->metaData.isEmpty()) {
        writer.writeTextElement(Mc::Constant::Tag::Xml::METADATA, d->metaData);
        return;
    }
    writer.writeStartElement(Mc::Constant::Tag::Xml::CHECKER);
    if (!d->className.isEmpty()) {
        writer.writeAttribute(Mc::Constant::Tag::Xml::CLASS, d->className);
    } else if (!d->refName.isEmpty()) {
        writer.writeAttribute(Mc::Constant::Tag::Xml::REF, d->refName);
    }
    writer.writeEndElement();
}
} // namespace Mc::XmlBuilder
