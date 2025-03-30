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
#include "McPluginBean.h"

#include <QXmlStreamWriter>

#include "McIocConstantGlobal.h"

namespace Mc::XmlBuilder {
MC_DECL_PRIVATE_DATA(PluginBean)
QString pluginPath;
MC_DECL_PRIVATE_DATA_END

PluginBean::PluginBean() noexcept
{
    MC_NEW_PRIVATE_DATA(PluginBean);
}

PluginBean::PluginBean(const QString &path) noexcept
    : PluginBean()
{
    d->pluginPath = path;
}

PluginBean::PluginBean(const QString &path, const QString &beanName) noexcept
    : PluginBean(path)
{
    setBeanName(beanName);
}

PluginBean::~PluginBean() {}

void PluginBean::setPluginPath(const QString &path) noexcept
{
    d->pluginPath = path;
}

void PluginBean::completeAttribute(QXmlStreamWriter &writer) const noexcept
{
    if (!d->pluginPath.isEmpty()) {
        writer.writeAttribute(Mc::Constant::Tag::Xml::PLUGIN, d->pluginPath);
    }
}
} // namespace Mc::XmlBuilder
