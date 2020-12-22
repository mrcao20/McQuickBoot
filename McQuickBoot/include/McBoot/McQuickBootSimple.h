#pragma once

#include "IMcQuickBoot.h"

#include "McBoot/Controller/impl/McCppResponse.h"
#include "McBoot/Requestor/McCppRequestor.h"

//! 此宏所对应的对象将在Application析构时销毁，所以一旦Application开始析构，就再也不要调用此宏
#define $ (McQuickBootSimple::instance()->requestor())

MC_FORWARD_DECL_CLASS(IMcApplicationContext);

MC_FORWARD_DECL_PRIVATE_DATA(McQuickBootSimple);

class MCQUICKBOOT_EXPORT McQuickBootSimple : public QObject, public IMcQuickBoot
{
    Q_OBJECT
    MC_DECL_INIT(McQuickBootSimple);
public:
    explicit McQuickBootSimple(QObject *parent = nullptr);
    ~McQuickBootSimple() override;
    
    static void init() noexcept;
    static QSharedPointer<McQuickBootSimple> instance() noexcept;

    McCppRequestor &requestor() const noexcept;
    IMcApplicationContextPtr getApplicationContext() const noexcept override;

private:
    MC_DECL_PRIVATE(McQuickBootSimple)
};

MC_DECL_POINTER(McQuickBootSimple);
