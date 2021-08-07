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

#include "../McLogMacroGlobal.h"

MC_FORWARD_DECL_CLASS(IMcApplicationContext);

class MCLOGQT_EXPORT McXMLConfigurator
{
public:
    static IMcApplicationContextPtr configure(
        const QString &path,
        const QString &flag = QString(),
        const QString &beanName = "defaultLoggerRepository") noexcept;
    static IMcApplicationContextPtr configure(
        const QStringList &paths,
        const QString &flag = QString(),
        const QString &beanName = "defaultLoggerRepository") noexcept;
    static void configure(IMcApplicationContextConstPtrRef appCtx,
                          const QString &beanName = "defaultLoggerRepository") noexcept;

private:
    IMcApplicationContextPtr doConfigure(const QStringList &paths,
                                         const QString &flag,
                                         const QString &beanName) noexcept;
    void doConfigure(IMcApplicationContextConstPtrRef appCtx, const QString &beanName) noexcept;
};
