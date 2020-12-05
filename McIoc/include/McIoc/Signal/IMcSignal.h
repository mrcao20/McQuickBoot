#pragma once

#include "../McGlobal.h"

/*!
 * \brief The IMcSignal class
 * 
 * 此类暂时无实际作用，仅仅标识某一个类拥有信号。
 * 建议所有拥有信号的类都实现该接口。
 * 后续可能会使用
 */
class IMcSignal
{
public:
    virtual ~IMcSignal() = default;
};

MC_DECL_POINTER(IMcSignal)
