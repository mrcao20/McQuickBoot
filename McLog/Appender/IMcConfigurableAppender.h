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

#include "IMcAppender.h"

class IMcConfigurableAppender : public IMcAppender
{
    MC_INTERFACES(IMcAppender)
public:
    virtual QString threshold() const noexcept = 0;
    /*!
     * \brief setThreshold
     *
     * 设置输出等级，设置完成后会被自动转换为全小写
     * info > critical > warn > debug
     * \param val
     */
    virtual void setThreshold(const QString &val) noexcept = 0;

    virtual QList<QtMsgType> types() const noexcept = 0;
};

MC_DECL_METATYPE(IMcConfigurableAppender)
