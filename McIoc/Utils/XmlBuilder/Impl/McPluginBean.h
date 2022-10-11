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
MC_FORWARD_DECL_PRIVATE_DATA(PluginBean);

class MC_IOC_EXPORT PluginBean : public Bean
{
public:
    PluginBean() noexcept;
    explicit PluginBean(const QString &path) noexcept;
    PluginBean(const QString &path, const QString &beanName) noexcept;
    ~PluginBean();

    void setPluginPath(const QString &path) noexcept;

protected:
    void completeAttribute(QXmlStreamWriter &writer) const noexcept override;

private:
    MC_DECL_PRIVATE(PluginBean)
};

MC_DECL_POINTER(PluginBean)
} // namespace Mc::XmlBuilder
