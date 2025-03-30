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

#include "../IMcConfigurableAppender.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractAppender);

class MC_LOG_EXPORT McAbstractAppender
    : public QObject
    , public IMcConfigurableAppender
    , public IMcBeanBuildable

{
    Q_OBJECT
    MC_INTERFACES(IMcConfigurableAppender)
    Q_PROPERTY(QString threshold READ threshold WRITE setThreshold)
public:
    McAbstractAppender() noexcept;
    ~McAbstractAppender();

    QString threshold() const noexcept override;
    void setThreshold(const QString &val) noexcept override;

    QList<QtMsgType> types() const noexcept override;

private:
    QList<QtMsgType> initThreshold(const QString &val) const noexcept;
    int strToEnum(const QString &val) const noexcept;

private:
    MC_DECL_PRIVATE(McAbstractAppender)
};

MC_DECL_POINTER(McAbstractAppender)
