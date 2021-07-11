#pragma once

#include "../../McBootGlobal.h"

MC_FORWARD_DECL_CLASS(IMcQuickBoot)

MC_FORWARD_DECL_PRIVATE_DATA(McServiceContainer)

class MCQUICKBOOT_EXPORT McServiceContainer : public QObject
{
    Q_OBJECT
    MC_DECL_INIT(McServiceContainer)
    MC_COMPONENT("serviceContainer")
public:
    McServiceContainer() noexcept;
    ~McServiceContainer() override;

    void init(const IMcQuickBoot *boot) noexcept;

private:
    void parseTimer(const QVariant &var) noexcept;
    void parseLongLiveThread(const QVariant &var) noexcept;

private:
    MC_DECL_PRIVATE(McServiceContainer)
};

MC_DECL_METATYPE(McServiceContainer)
