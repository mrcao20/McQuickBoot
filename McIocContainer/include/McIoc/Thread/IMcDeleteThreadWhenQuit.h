#pragma once

#include "../McGlobal.h"

/*!
 * \brief The IMcDeleteThreadWhenQuit class
 * 
 * 当从IOC容器中实例化一个实现至本接口的对象时，会调用本接口的deleteWhenQuit方法
 */
class IMcDeleteThreadWhenQuit 
{
    MC_DEFINE_TYPELIST()
public:
    virtual ~IMcDeleteThreadWhenQuit() = default;
    
    virtual void deleteWhenQuit() noexcept = 0;
};

MC_DECL_METATYPE(IMcDeleteThreadWhenQuit)
