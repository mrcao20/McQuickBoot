#pragma once

#include "../McGlobal.h"

class IMcRefreshableApplicationContext 
{
public:
    virtual ~IMcRefreshableApplicationContext() = default;
    
    /*!
     * \brief refresh
     * 
     * 刷新上下文，初始化上下文内容，同时初始化bean容器，使bean容器中对象全部加载
     * \param thread
     */
    virtual void refresh(QThread *thread = nullptr) noexcept = 0;
};

MC_DECL_POINTER(IMcRefreshableApplicationContext)
