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
#include "../IMcPropertyContent.h"

namespace Mc::XmlBuilder {
MC_FORWARD_DECL_PRIVATE_DATA(List);

class MC_IOC_EXPORT List
    : public IPropertyContent
    , public IBeanContent
{
public:
    List() noexcept;
    ~List();

    void addContent(const IPropertyContentPtr &val) noexcept;
    void addContent(const QString &val) noexcept;

    void write(QXmlStreamWriter &writer) const noexcept override;

private:
    MC_DECL_PRIVATE(List)
};

MC_DECL_POINTER(List)
} // namespace Mc::XmlBuilder
