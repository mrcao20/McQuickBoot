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
