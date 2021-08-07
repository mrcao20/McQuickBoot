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

#include "../McBootGlobal.h"

/*!
 * \brief The IMcServiceLongLiveThread class
 * 实现至此接口的Service对象将会改变其生存线程，线程由框架内部持有。
 * \note 由于此接口改变生存线程时对象早已构造完成，所以该生存线程对MC_ALL_FINISHED标志无效。
 * \note 大部分情况下你都不需要使用此接口，因为QuickBoot框架提供在MC_CONFIGURATION对象中指定
 * 所有对象生存线程的功能。但有时一些长时任务也许需要使用此接口。
 */
class IMcServiceLongLiveThread
{
public:
    MC_BASE_DESTRUCTOR(IMcServiceLongLiveThread)

    /*!
     * \brief moveFinished
     * 当生存线程移动完毕时调用此方法，调用线程为移动后的生存线程。
     */
    virtual void moveFinished() noexcept {}
};

MC_DECL_METATYPE(IMcServiceLongLiveThread)
