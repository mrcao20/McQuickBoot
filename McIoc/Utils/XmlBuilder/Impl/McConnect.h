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
MC_FORWARD_DECL_PRIVATE_DATA(Connect);

class MC_IOC_EXPORT Connect : public IBeanContent
{
public:
    Connect() noexcept;
    ~Connect();

    void setSender(const QString &val) noexcept;
    void setSignal(const QString &val) noexcept;
    void setReceiver(const QString &val) noexcept;
    void setSlot(const QString &val) noexcept;
    void setConnectionType(const QString &val) noexcept;

    void write(QXmlStreamWriter &writer) const noexcept override;

private:
    MC_DECL_PRIVATE(Connect)
};

MC_DECL_POINTER(Connect)
} // namespace Mc::XmlBuilder
