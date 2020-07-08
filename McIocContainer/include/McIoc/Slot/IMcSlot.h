#pragma once

#include "../McGlobal.h"

/*!
 * \brief The IMcSlot class
 * 
 * 此类暂时无实际作用，仅仅标识某一个类拥有槽。
 * 建议所有拥有槽的类都实现该接口。
 * 后续可能会使用
 */
class IMcSlot
{
public:
    virtual ~IMcSlot() = default;
};

MC_DECL_POINTER(IMcSlot)
