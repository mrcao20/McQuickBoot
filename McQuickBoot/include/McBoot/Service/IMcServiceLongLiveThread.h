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
