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

#include "../IMcBean.h"
#include "../IMcPropertyContent.h"

namespace Mc::XmlBuilder {
MC_FORWARD_DECL_CLASS(IBeanContent);

MC_FORWARD_DECL_PRIVATE_DATA(Bean);

class MC_IOC_EXPORT Bean
    : public IBean
    , public IPropertyContent
{
public:
    Bean() noexcept;
    explicit Bean(const QString &beanName) noexcept;
    ~Bean();

    void setBeanName(const QString &name) noexcept;
    void setSingleton(bool val) noexcept;
    void setPointer(bool val) noexcept;
    void setParent(const QString &name) noexcept;
    void setFlag(const QString &val) noexcept;
    void addContent(const IBeanContentPtr &content) noexcept;

    void write(QXmlStreamWriter &writer) const noexcept override;

protected:
    virtual void completeAttribute(QXmlStreamWriter &writer) const noexcept = 0;

private:
    MC_DECL_PRIVATE(Bean)
};

MC_DECL_POINTER(Bean)
} // namespace Mc::XmlBuilder
