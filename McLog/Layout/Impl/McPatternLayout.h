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
#pragma once

#include <McIoc/Utils/IMcBeanBuildable.h>

#include "../IMcLayout.h"

MC_FORWARD_DECL_PRIVATE_DATA(McPatternLayout);

class MC_LOG_EXPORT McPatternLayout
    : public QObject
    , public IMcLayout
    , public IMcBeanBuildable
{
    Q_OBJECT
    MC_FULL_DEFINE(McPatternLayout, QObject, IMcLayout)
    Q_PROPERTY(QString pattern READ pattern WRITE setPattern)
public:
    McPatternLayout() noexcept;
    ~McPatternLayout();

    QString pattern() const noexcept;
    void setPattern(const QString &val) noexcept;

    QString format(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept override;

private:
    MC_DECL_PRIVATE(McPatternLayout)
};

MC_DECL_METATYPE(McPatternLayout)
