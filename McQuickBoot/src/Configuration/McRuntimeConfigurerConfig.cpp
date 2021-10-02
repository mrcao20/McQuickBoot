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
#include "McBoot/Configuration/McRuntimeConfigurerConfig.h"

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(McRuntimeConfigurerConfig)
MC_STATIC_END

MC_DECL_PRIVATE_DATA(McRuntimeConfigurerConfig)
QString filePath;
MC_DECL_PRIVATE_DATA_END

McRuntimeConfigurerConfig::McRuntimeConfigurerConfig(QObject *parent) noexcept : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McRuntimeConfigurerConfig);
}

McRuntimeConfigurerConfig::~McRuntimeConfigurerConfig() {}

QString McRuntimeConfigurerConfig::filePath() const noexcept
{
    return d->filePath;
}

void McRuntimeConfigurerConfig::setFilePath(const QString &val) noexcept
{
    d->filePath = val;
}
