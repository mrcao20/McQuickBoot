#pragma once

#include "McAbstractRequestor.h"

MC_FORWARD_DECL_PRIVATE_DATA(McCppRequestor);

class McCppResponse;

class MCQUICKBOOT_EXPORT McCppRequestor : public McAbstractRequestor
{
    Q_OBJECT
    MC_DECL_INIT(McCppRequestor)
    MC_DEFINE_TYPELIST(MC_TYPELIST(McAbstractRequestor))
    MC_COMPONENT
    MC_BEANNAME("cppRequestor")
    //! 在容器中为非单例，但是McQuickBoot会控制其为单例
    MC_SINGLETON(false)
public:
    Q_INVOKABLE explicit McCppRequestor(QObject *parent = nullptr);
    ~McCppRequestor() override;

    McCppResponse &invoke(const QString &uri) noexcept;
    McCppResponse &invoke(const QString &uri, const QJsonObject &data) noexcept;

private:
    MC_DECL_PRIVATE(McCppRequestor)
};

MC_DECL_METATYPE(McCppRequestor)
