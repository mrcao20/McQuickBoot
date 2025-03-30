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

#include "../../McIocConstantGlobal.h"
#include "../../McIocGlobal.h"

MC_FORWARD_DECL_CLASS(McBeanEnum)

class MC_IOC_EXPORT McBeanConnector
{
public:
    QString sender() const noexcept { return m_sender; }
    void setSender(const QString &sender) noexcept { m_sender = sender.simplified(); }

    QString signal() const noexcept { return m_signal; }
    void setSignal(const QString &signal) noexcept
    {
        m_signal = QMetaObject::normalizedSignature(signal.toLocal8Bit());
    }

    QString receiver() const noexcept { return m_receiver; }
    void setReceiver(const QString &receiver) noexcept { m_receiver = receiver.simplified(); }

    QString slot() const noexcept { return m_slot; }
    void setSlot(const QString &slot) noexcept { m_slot = QMetaObject::normalizedSignature(slot.toLocal8Bit()); }

    McBeanEnumPtr type() const noexcept { return m_type; }
    void setType(const McBeanEnumPtr &type) noexcept { m_type = type; }

private:
    QString m_sender{Mc::Constant::Tag::Xml::THIS}; //!< 发送方beanName，默认为对象本身
    QString m_signal; //!< 信号名
    QString m_receiver{Mc::Constant::Tag::Xml::THIS}; //!< 接收方beanName，默认为对象本身
    QString m_slot; //!< 槽名
    McBeanEnumPtr m_type; //!< 连接方式，默认为自动连接
};

MC_DECL_POINTER(McBeanConnector)
