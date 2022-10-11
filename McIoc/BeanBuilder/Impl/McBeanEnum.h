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

#include "../../McIocGlobal.h"

class MC_IOC_EXPORT McBeanEnum
{
public:
    QString scope() const noexcept { return m_scope; }
    void setScope(const QString &val) noexcept { m_scope = val; }

    QString type() const noexcept { return m_type; }
    void setType(const QString &val) noexcept { m_type = val; }

    QString value() const noexcept { return m_value; }
    void setValue(const QString &val) noexcept { m_value = val; }

private:
    QString m_scope;
    QString m_type;
    QString m_value;
};

MC_DECL_METATYPE(McBeanEnum)
