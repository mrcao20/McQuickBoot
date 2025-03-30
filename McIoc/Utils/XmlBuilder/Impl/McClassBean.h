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

#include "McBean.h"

namespace Mc::XmlBuilder {
MC_FORWARD_DECL_PRIVATE_DATA(ClassBean);

class MC_IOC_EXPORT ClassBean : public Bean
{
public:
    ClassBean() noexcept;
    explicit ClassBean(const QString &name) noexcept;
    ClassBean(const QString &className, const QString &beanName) noexcept;
    ~ClassBean();

    void setClassName(const QString &name) noexcept;

protected:
    void completeAttribute(QXmlStreamWriter &writer) const noexcept override;

private:
    MC_DECL_PRIVATE(ClassBean)
};

MC_DECL_POINTER(ClassBean)
} // namespace Mc::XmlBuilder
