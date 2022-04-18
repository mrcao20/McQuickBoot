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
#include "McIocGlobal.h"

#include <QHash>

Q_LOGGING_CATEGORY(mcIoc, "mc.ioc")

MC_GLOBAL_STATIC_BEGIN(staticData)
QHash<McMetaType, QSet<QString>> metaTypeBeanNames;
MC_GLOBAL_STATIC_END(staticData)

namespace McPrivate {
void addMetaTypeBeanName(const McMetaType &type, const QString &beanName) noexcept
{
    addMetaTypeBeanName(QVector<McMetaType>{type}, beanName);
}

void addMetaTypeBeanName(const QVector<McMetaType> &types, const QString &beanName) noexcept
{
    for (auto &type: types) {
        staticData->metaTypeBeanNames[type].insert(beanName);
    }
}

QSet<QString> getBeanNameForMetaType(const McMetaType &type) noexcept
{
    return staticData->metaTypeBeanNames.value(type);
}
} // namespace McPrivate

namespace Mc {
bool isContainedTag(const QByteArray &tags, const QByteArray &tag) noexcept
{
    auto tagList = tags.split(' ');
    for (auto &t: qAsConst(tagList)) {
        if (t == tag) {
            return true;
        }
    }
    return false;
}
} // namespace Mc
