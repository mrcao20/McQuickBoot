#pragma once

#include "../../McBootGlobal.h"

class IMcResponseHandler
{
public:
    virtual ~IMcResponseHandler() = default;

    /*!
     * \brief handler
     * 预处理响应体
     * \param body Controller的返回值
     * \return 返回true则继续向下执行。返回false则中断执行，后续的then/error等都不会被执行
     */
    virtual bool handler(QVariant &body) noexcept = 0;
};

MC_DECL_METATYPE(IMcResponseHandler)
