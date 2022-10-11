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

#include "../IMcBeanContent.h"

namespace Mc::XmlBuilder {
MC_FORWARD_DECL_CLASS(IPropertyContent);

MC_FORWARD_DECL_PRIVATE_DATA(ConstructorArg);

class MC_IOC_EXPORT ConstructorArg : public IBeanContent
{
public:
    ConstructorArg() noexcept;
    ConstructorArg(int index, const IPropertyContentPtr &val) noexcept;
    ConstructorArg(int index, const QString &value) noexcept;
    ConstructorArg(const QString &name, const IPropertyContentPtr &val) noexcept;
    ConstructorArg(const QString &name, const QString &value) noexcept;
    ~ConstructorArg();

    void setIndex(int val) noexcept;
    void setName(const QString &name) noexcept;
    void setContent(const IPropertyContentPtr &val) noexcept;
    void setContent(const QString &value) noexcept;

    void write(QXmlStreamWriter &writer) const noexcept override;

private:
    MC_DECL_PRIVATE(ConstructorArg)
};

MC_DECL_POINTER(ConstructorArg)
} // namespace Mc::XmlBuilder
