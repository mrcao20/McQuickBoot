#pragma once

#include "McAbstractQuickBoot.h"

MC_FORWARD_DECL_CLASS(IMcApplicationContext);

MC_FORWARD_DECL_PRIVATE_DATA(McQuickBootSimple);

class MCQUICKBOOT_EXPORT McQuickBootSimple : public McAbstractQuickBoot
{
    Q_OBJECT
    MC_DECL_INIT(McQuickBootSimple)
public:
    explicit McQuickBootSimple(QObject *parent = nullptr);
    ~McQuickBootSimple() override;

    static void init() noexcept;
    static QSharedPointer<McQuickBootSimple> instance() noexcept;

    //! 如果重新或新加载了某些组件，则调用此函数
    void refresh() noexcept;
    IMcApplicationContextPtr getApplicationContext() const noexcept override;

private:
    void doRefresh() noexcept;
    void initContainer() const noexcept;

private:
    MC_DECL_PRIVATE(McQuickBootSimple)
};

MC_DECL_POINTER(McQuickBootSimple);
