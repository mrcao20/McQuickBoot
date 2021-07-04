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
