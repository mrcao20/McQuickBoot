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
#ifdef MC_USE_QT5
    setMetaType(McMetaType::fromPTypeName(className + "*"));
#else
    setMetaType(McMetaType::fromTypeName(className));
#endif
    auto beanStar = obj->qt_metacast(className);
    QVariant beanVar(metaType().pMetaType(), &beanStar);
    return beanVar;
}
