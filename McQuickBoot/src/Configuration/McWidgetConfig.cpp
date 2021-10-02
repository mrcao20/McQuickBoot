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
#include "McBoot/Configuration/McWidgetConfig.h"

#include <McWidgetIoc/ApplicationContext/Impl/McXmlWidgetApplicationContext.h>

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(McWidgetConfig)
MC_STATIC_END

MC_DECL_PRIVATE_DATA(McWidgetConfig)
IMcWidgetApplicationContextPtr appCtx;
QString mainWindowName{"mainWindow"};
int destroyPriority{Mc::Normal};
MC_DECL_PRIVATE_DATA_END

McWidgetConfig::McWidgetConfig(QObject *parent) noexcept : McAbstractXmlPathConfig(parent)
{
    MC_NEW_PRIVATE_DATA(McWidgetConfig);
}

McWidgetConfig::~McWidgetConfig() {}

QString McWidgetConfig::mainWindowName() const noexcept
{
    return d->mainWindowName;
}

void McWidgetConfig::setMainWindowName(const QString &val) noexcept
{
    d->mainWindowName = val;
}

int McWidgetConfig::destroyPriority() const noexcept
{
    return d->destroyPriority;
}

void McWidgetConfig::setDestroyPriority(int val) noexcept
{
    d->destroyPriority = val;
}

IMcWidgetApplicationContextPtr McWidgetConfig::appCtx() const noexcept
{
    return d->appCtx;
}

void McWidgetConfig::doFinished() noexcept
{
    super::doFinished();
    auto paths = xmlPaths();
    if (paths.isEmpty()) {
        return;
    }
    QStringList xmlPaths;
    for (auto &path : qAsConst(paths)) {
        auto xmlPath = Mc::toAbsolutePath(path);
        xmlPaths << xmlPath;
    }
    d->appCtx = McXmlWidgetApplicationContextPtr::create(xmlPaths, flag());
}
