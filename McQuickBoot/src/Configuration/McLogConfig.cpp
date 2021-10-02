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
#include "McBoot/Configuration/McLogConfig.h"

#include <McIoc/ApplicationContext/impl/McLocalPathApplicationContext.h>

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(McLogConfig)
MC_STATIC_END

MC_DECL_PRIVATE_DATA(McLogConfig)
IMcApplicationContextPtr appCtx;
QString repositoryName{"defaultLoggerRepository"};
MC_DECL_PRIVATE_DATA_END

McLogConfig::McLogConfig(QObject *parent) noexcept : McAbstractXmlPathConfig(parent)
{
    MC_NEW_PRIVATE_DATA(McLogConfig);
}

McLogConfig::~McLogConfig() {}

QString McLogConfig::repositoryName() const noexcept
{
    return d->repositoryName;
}

void McLogConfig::setRepositoryName(const QString &val) noexcept
{
    d->repositoryName = val;
}

IMcApplicationContextPtr McLogConfig::appCtx() const noexcept
{
    return d->appCtx;
}

void McLogConfig::doFinished() noexcept
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
    d->appCtx = McLocalPathApplicationContextPtr::create(xmlPaths, flag());
}
