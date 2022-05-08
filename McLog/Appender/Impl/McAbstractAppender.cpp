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
#include "McAbstractAppender.h"

namespace {
constexpr const QLatin1String LEVEL_DEBUG{"debug", 5};
constexpr const QLatin1String LEVEL_WARN{"warn", 4};
constexpr const QLatin1String LEVEL_CRITICAL{"critical", 8};
constexpr const QLatin1String LEVEL_FATAL{"fatal", 5};
constexpr const QLatin1String LEVEL_INFO{"info", 4};
} // namespace

MC_DECL_PRIVATE_DATA(McAbstractAppender)
QString threshold; //!< 全小写
QList<QtMsgType> types;
MC_DECL_PRIVATE_DATA_END

McAbstractAppender::McAbstractAppender() noexcept
{
    MC_NEW_PRIVATE_DATA(McAbstractAppender);
}

McAbstractAppender::~McAbstractAppender() {}

QString McAbstractAppender::threshold() const noexcept
{
    return d->threshold;
}

void McAbstractAppender::setThreshold(const QString &val) noexcept
{
    d->threshold = val.toLower();

    d->types = initThreshold(d->threshold);
}

QList<QtMsgType> McAbstractAppender::types() const noexcept
{
    return d->types;
}

QList<QtMsgType> McAbstractAppender::initThreshold(const QString &val) const noexcept
{
    auto typeStr = val.simplified();
    QList<QtMsgType> types;

    if (!typeStr.endsWith('-') && !typeStr.startsWith('-')) {
        auto list = typeStr.split(',');
        for (auto str : list) {
            auto type = strToEnum(str.simplified());
            if (type != -1) {
                types.append(static_cast<QtMsgType>(type));
            }
        }
        return types;
    } else if (typeStr.startsWith('-')) {
        auto level = typeStr.simplified();
        level.remove('-');
        auto type = strToEnum(level);

        types.append(static_cast<QtMsgType>(type));
        if (level == LEVEL_DEBUG)
            return types;
        types.append(QtMsgType::QtDebugMsg);
        if (level == LEVEL_WARN)
            return types;
        types.append(QtMsgType::QtWarningMsg);
        if (level == LEVEL_CRITICAL)
            return types;
        types.append(QtMsgType::QtCriticalMsg);
        if (level == LEVEL_FATAL)
            return types;
        types.append(QtMsgType::QtFatalMsg);
        if (level != LEVEL_INFO) {
            types.clear();
            return types;
        }

        return types;
    } else {
        auto level = typeStr.simplified();
        level.remove('-');
        auto type = strToEnum(level);

        types.append(static_cast<QtMsgType>(type));
        if (level == LEVEL_INFO)
            return types;
        types.append(QtMsgType::QtInfoMsg);
        if (level == LEVEL_FATAL)
            return types;
        types.append(QtMsgType::QtFatalMsg);
        if (level == LEVEL_CRITICAL)
            return types;
        types.append(QtMsgType::QtCriticalMsg);
        if (level == LEVEL_WARN)
            return types;
        types.append(QtMsgType::QtWarningMsg);
        if (level != LEVEL_DEBUG) {
            types.clear();
            return types;
        }

        return types;
    }
}

int McAbstractAppender::strToEnum(const QString &val) const noexcept
{
    auto type = val.simplified();
    if (type == LEVEL_DEBUG)
        return QtMsgType::QtDebugMsg;
    else if (type == LEVEL_WARN)
        return QtMsgType::QtWarningMsg;
    else if (type == LEVEL_CRITICAL)
        return QtMsgType::QtCriticalMsg;
    else if (type == LEVEL_FATAL)
        return QtMsgType::QtFatalMsg;
    else if (type == LEVEL_INFO)
        return QtMsgType::QtInfoMsg;
    return -1;
}
