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

#include "../../../McIocGlobal.h"

namespace Mc::XmlBuilder {
MC_FORWARD_DECL_CLASS(IBean);

MC_FORWARD_DECL_PRIVATE_DATA(BeanCollection);

class MC_IOC_EXPORT BeanCollection
{
public:
    BeanCollection() noexcept;
    ~BeanCollection();

    void addBean(const IBeanPtr &bean) noexcept;
    void addBeans(const QList<IBeanPtr> &beans) noexcept;

    void writeToDevice(QIODevice *dev, int indent = 4) const noexcept;

private:
    MC_DECL_PRIVATE(BeanCollection)
};

MC_DECL_POINTER(BeanCollection)
} // namespace Mc::XmlBuilder
