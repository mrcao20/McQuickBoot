#pragma once

#include "../IMcDaoLoader.h"

#include "../../McOrmGlobal.h"

MC_FORWARD_DECL_PRIVATE_DATA(McDaoLoader);

class MCORM_EXPORT McDaoLoader : public QObject, public IMcDaoLoader
{
    Q_OBJECT
    MC_DEFINE_TYPELIST(IMcDaoLoader)
    MC_DECL_INIT(McDaoLoader)
    MC_COMPONENT
    MC_BEANNAME("daoLoader")
    MC_SINGLETON(true)
public:
    Q_INVOKABLE McDaoLoader() noexcept;
    ~McDaoLoader() override;
    
    void init() noexcept;
    void init(IMcApplicationContextConstPtrRef appCtx) noexcept;
    
private:
    MC_DECL_PRIVATE(McDaoLoader)
};

MC_DECL_METATYPE(McDaoLoader)
