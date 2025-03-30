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
MC_FORWARD_DECL_PRIVATE_DATA(Map);

class MC_IOC_EXPORT Map
    : public IPropertyContent
    , public IBeanContent
{
public:
    Map() noexcept;
    ~Map();

    void addContent(const IPropertyContentPtr &key, const IPropertyContentPtr &value) noexcept;
    void addContent(const QString &key, const IPropertyContentPtr &value) noexcept;
    void addContent(const IPropertyContentPtr &key, const QString &value) noexcept;
    void addContent(const QString &key, const QString &value) noexcept;

    void write(QXmlStreamWriter &writer) const noexcept override;

private:
    MC_DECL_PRIVATE(Map)
};

MC_DECL_POINTER(Map)
} // namespace Mc::XmlBuilder
