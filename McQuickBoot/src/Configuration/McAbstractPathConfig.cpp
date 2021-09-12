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
#include "McBoot/Configuration/McAbstractPathConfig.h"

MC_DECL_PRIVATE_DATA(McAbstractPathConfig)
QString dirPath;
QStringList filters;
QStringList filterPaths;
MC_DECL_PRIVATE_DATA_END

McAbstractPathConfig::McAbstractPathConfig(QObject *parent) noexcept : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractPathConfig);
}

McAbstractPathConfig::~McAbstractPathConfig() {}

QStringList McAbstractPathConfig::filterPaths() const noexcept
{
    return d->filterPaths;
}

void McAbstractPathConfig::doFinished() noexcept
{
    if (d->dirPath.isEmpty()) {
        return;
    }
    auto path = Mc::toAbsolutePath(d->dirPath);
    QDir dir(path);
    auto fileInfos = dir.entryInfoList(QDir::Files);
    for (auto &fileInfo : qAsConst(fileInfos)) {
        auto absPath = fileInfo.absoluteFilePath();
        if (!fileInfo.isFile()) {
            continue;
        }
        if (!fileNameCheck(fileInfo.fileName())) {
            continue;
        }
        d->filterPaths.append(absPath);
    }
}

void McAbstractPathConfig::allFinished() noexcept
{
    doFinished();
}

bool McAbstractPathConfig::fileNameCheck(const QString &fileName) noexcept
{
    if (d->filters.isEmpty()) {
        return true;
    }
    for (auto &filter : qAsConst(d->filters)) {
        QRegularExpression reg(filter);
        auto match = reg.match(fileName);
        if (!match.capturedTexts().isEmpty()) {
            return true;
        }
    }
    return false;
}

#include "moc_McAbstractPathConfig.cpp"
