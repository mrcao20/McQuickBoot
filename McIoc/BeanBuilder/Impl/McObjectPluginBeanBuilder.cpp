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
#include "McObjectPluginBeanBuilder.h"

#include <McCore/PluginChecker/IMcPluginChecker.h>

#include "../IMcBeanReferenceResolver.h"

MC_DECL_PRIVATE_DATA(McObjectPluginBeanBuilder)
QString pluginPath;
McBeanReferencePtr pluginChecker;
MC_DECL_PRIVATE_DATA_END

McObjectPluginBeanBuilder::McObjectPluginBeanBuilder() noexcept
{
    MC_NEW_PRIVATE_DATA(McObjectPluginBeanBuilder);
}

McObjectPluginBeanBuilder::~McObjectPluginBeanBuilder() {}

void McObjectPluginBeanBuilder::setPluginPath(const QString &path) noexcept
{
    d->pluginPath = path;
}

void McObjectPluginBeanBuilder::setPluginChecker(const McBeanReferencePtr &ref) noexcept
{
    d->pluginChecker = ref;
}

//! 插件必须是单例
bool McObjectPluginBeanBuilder::isSingleton() const noexcept
{
    return true;
}

bool McObjectPluginBeanBuilder::isPointer() const noexcept
{
    return true;
}

QVariant McObjectPluginBeanBuilder::create() noexcept
{
    std::function<bool(const QJsonObject &)> checker = nullptr;
    if (!d->pluginChecker.isNull()) {
        auto c = resolver()->resolveBeanReference(d->pluginChecker).value<IMcPluginCheckerPtr>();
        if (!c.isNull()) {
            checker = [c](const QJsonObject &json) { return c->check(json); };
        }
    }
    auto obj = Mc::loadPlugin(d->pluginPath, checker);
    if (obj == nullptr) {
        return QVariant();
    }
    QByteArray className = obj->metaObject()->className();
    setMetaType(McMetaType::fromTypeName(className));
    auto beanStar = obj->qt_metacast(className);
    QVariant beanVar(metaType().pMetaType(), &beanStar);
    return beanVar;
}
