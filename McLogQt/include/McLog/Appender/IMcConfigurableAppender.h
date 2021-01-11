#pragma once

#include "IMcAppender.h"
#include "IMcWritableAppender.h"

class IMcConfigurableAppender 
        : public IMcAppender 
{
    MC_TYPELIST(IMcAppender)
public:
    virtual ~IMcConfigurableAppender() = default;
    
    virtual QString threshold() const noexcept = 0;
    /*!
     * \brief setThreshold
     * 
     * 设置输出等级，设置完成后会被自动转换为全小写
     * info > critical > warn > debug
     * \param val
     */
    virtual void setThreshold(const QString &val) noexcept = 0;
    
    virtual QList<QtMsgType> types() const noexcept = 0;
};

MC_DECL_METATYPE(IMcConfigurableAppender)
