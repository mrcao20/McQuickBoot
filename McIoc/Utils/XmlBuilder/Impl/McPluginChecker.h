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
MC_FORWARD_DECL_PRIVATE_DATA(PluginChecker);

class MC_IOC_EXPORT PluginChecker : public IBeanContent
{
public:
    PluginChecker() noexcept;
    ~PluginChecker();

    void setMetaData(const QString &val) noexcept;
    void setClassName(const QString &val) noexcept;
    void setRefName(const QString &val) noexcept;

    void write(QXmlStreamWriter &writer) const noexcept override;

private:
    MC_DECL_PRIVATE(PluginChecker)
};

MC_DECL_POINTER(PluginChecker)
} // namespace Mc::XmlBuilder
