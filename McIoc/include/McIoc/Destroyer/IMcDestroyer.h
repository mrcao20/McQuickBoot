#pragma once

#include "../McGlobal.h"

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
    virtual ~IMcDestroyer() = default;
    
    virtual void destroy() noexcept = 0;
};

MC_DECL_METATYPE(IMcDestroyer)

namespace Mc {

struct McCustomDeleter
{
    void operator()(QObject *ptr)
    {
//        //! 这里用到了dynamic_cast，原则上来说不应该使用这种方法
//        //! 暂时没有更好的解决方案，后续处理
//        auto d = dynamic_cast<IMcDestroyer *>(ptr);
//        if(d == nullptr) {
//            delete ptr;
//        } else {
//            d->destroy();
//        }
        //! 取消dynamic_cast
        auto d = ptr->property(Mc::Constant::Property::customDeleter).value<IMcDestroyer *>();
        if(d != nullptr) {
            d->destroy();
        } else {
            delete ptr;
        }
    }
};

}
