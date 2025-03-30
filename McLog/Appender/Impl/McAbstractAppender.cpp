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
