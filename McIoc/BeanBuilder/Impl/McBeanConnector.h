/*
 * MIT License
 *
 * Copyright (c) 2021 mrcao20
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once

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
