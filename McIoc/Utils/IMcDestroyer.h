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

#include "../McIocGlobal.h"

/*!
 * \brief The IMcDestroyer class
 * 
 * 对象销毁器，当对象实现至该接口时，会将destroy函数传入QSharedPointer
 * 即，当引用计数为0时，QSharedPointer的析构器会调用destroy函数。
 * 此接口用于自定义删除对象。
 */
class IMcDestroyer
{
public:
    MC_BASE_DESTRUCTOR(IMcDestroyer)

    virtual void destroy() noexcept = 0;
};

MC_DECL_METATYPE(IMcDestroyer)

namespace Mc {
struct McCustomDeleter
{
    void operator()(QObject *ptr)
    {
        //! 这里用到了dynamic_cast，原则上来说不应该使用这种方法
        //! 暂时没有更好的解决方案，后续处理
        auto d = dynamic_cast<IMcDestroyer *>(ptr);
        if (d == nullptr) {
            delete ptr;
        } else {
            d->destroy();
        }
        //! 取消dynamic_cast
        //! 2021/2/4 由于setProperty和property线程不安全，且setProperty内部使用了sendEvent，所以取消这种方法
        //        auto d = ptr->property(Mc::Constant::Property::customDeleter).value<IMcDestroyer *>();
        //        if(d != nullptr) {
        //            d->destroy();
        //        } else {
        //            delete ptr;
        //        }
    }
};
} // namespace Mc
