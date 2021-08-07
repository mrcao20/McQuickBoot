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
 * \brief The IMcServiceTimer class
 * 实现至此接口的Service对象将会定时调用该接口中的方法，且调用线程为该Service对象的生存线程
 * \see timeout
 * \see interval
 */
class IMcServiceTimer
{
public:
    MC_BASE_DESTRUCTOR(IMcServiceTimer)

    /*!
     * \brief timeout
     * 每隔固定时间就会调用本方法，且调用本方法的线程为实现此接口的对象的生存线程
     * \see interval
     */
    virtual void timeout() noexcept = 0;
    /*!
     * \brief interval
     * 此方法指代定时器的超时时长，每间隔固定毫秒数即超时一次
     * 
     * \return 超时时长，单位：ms
     */
    virtual int interval() noexcept = 0;
};

MC_DECL_METATYPE(IMcServiceTimer)
