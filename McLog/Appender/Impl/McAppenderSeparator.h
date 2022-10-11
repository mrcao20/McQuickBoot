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

#include "../../McLogGlobal.h"

struct MC_LOG_EXPORT McAppenderSeparator
{
    Q_GADGET
public:
    MC_POCO_PROPERTY(QString, separator)

    MC_POCO_PROPERTY(bool, isAutoSeparate, {true})

    //! 单位：ms
    MC_POCO_PROPERTY(quint64, autoSeparateInterval, {300000})
};

MC_DECL_METATYPE(McAppenderSeparator)
