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
#include "McPatternLayout.h"

#include "Utils/McMessagePattern.h"

MC_AUTO_INIT(McPatternLayout)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McPatternLayout)
QString pattern;
McPrivate::MessagePatternPtr messagePattern;
MC_DECL_PRIVATE_DATA_END

McPatternLayout::McPatternLayout() noexcept
{
    MC_NEW_PRIVATE_DATA(McPatternLayout);

    d->messagePattern = McPrivate::MessagePatternPtr::create();
}

McPatternLayout::~McPatternLayout() {}

QString McPatternLayout::pattern() const noexcept
{
    return d->pattern;
}

void McPatternLayout::setPattern(const QString &val) noexcept
{
    d->pattern = val;

    d->messagePattern->setPattern(val);
}

QString McPatternLayout::format(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept
{
    return McPrivate::format(d->messagePattern, type, context, str);
}
