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

#include "McXmlWidgetApplicationContext.h"

class MC_WIDGET_EXPORT McLocalPathWidgetApplicationContext : public McXmlWidgetApplicationContext
{
public:
    McLocalPathWidgetApplicationContext() noexcept;
    explicit McLocalPathWidgetApplicationContext(const QString &location, const QString &flag = QString()) noexcept;
    explicit McLocalPathWidgetApplicationContext(const QStringList &locations, const QString &flag = QString()) noexcept;
};

MC_DECL_POINTER(McLocalPathWidgetApplicationContext)
