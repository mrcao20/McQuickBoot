#pragma once

#include "IMcQuickBoot.h"

#include "McBoot/Requestor/McQmlRequestor.h"
#include "McBoot/Controller/impl/McQmlResponse.h"

//! 此宏所对应的对象将在Application析构时销毁，所以一旦Application开始析构，就再也不要调用此宏
#define $ McQuickBootSimple::requestor()

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
    static McQmlRequestorPtr requestor() noexcept;
    
    IMcApplicationContextPtr getApplicationContext() const noexcept override;
    
private:
    MC_DECL_PRIVATE(McQuickBootSimple)
};

MC_DECL_POINTER(McQuickBootSimple);
