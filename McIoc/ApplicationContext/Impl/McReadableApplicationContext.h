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

#include "../IMcReadableApplicationContext.h"
#include "McAbstractApplicationContext.h"

MC_FORWARD_DECL_PRIVATE_DATA(McReadableApplicationContext);

class MC_IOC_EXPORT McReadableApplicationContext
    : public McAbstractApplicationContext
    , public IMcReadableApplicationContext
{
public:
    McReadableApplicationContext() noexcept;
    explicit McReadableApplicationContext(const IMcConfigurableBeanFactoryPtr &factory) noexcept;
    explicit McReadableApplicationContext(const IMcBeanBuilderReaderPtr &reader) noexcept;
    McReadableApplicationContext(
        const IMcConfigurableBeanFactoryPtr &factory, const IMcBeanBuilderReaderPtr &reader) noexcept;
    ~McReadableApplicationContext() override;

    void readBeans() noexcept override;

    void setReader(const IMcBeanBuilderReaderPtr &reader) noexcept override;

private:
    MC_DECL_PRIVATE(McReadableApplicationContext)
};

MC_DECL_POINTER(McReadableApplicationContext)
