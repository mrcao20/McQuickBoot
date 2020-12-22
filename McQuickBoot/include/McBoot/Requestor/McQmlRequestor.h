#pragma once

#include "McAbstractRequestor.h"

MC_FORWARD_DECL_PRIVATE_DATA(McQmlRequestor);

MC_FORWARD_DECL_CLASS(IMcQmlSocketContainer);

class McQmlResponse;
class McQmlSocket;

class MCQUICKBOOT_EXPORT McQmlRequestor : public McAbstractRequestor
{
    Q_OBJECT
    MC_DECL_INIT(McQmlRequestor)
    MC_DEFINE_TYPELIST(MC_TYPELIST(McAbstractRequestor))
    MC_COMPONENT
    MC_BEANNAME("qmlRequestor")
    //! 在容器中为非单例，但是McQuickBoot会控制其为单例
    MC_SINGLETON(false)
public:
    Q_INVOKABLE explicit McQmlRequestor(QObject *parent = nullptr);
    ~McQmlRequestor() override;

    void setSocketContainer(IMcQmlSocketContainerConstPtrRef val) noexcept;

    Q_INVOKABLE McQmlResponse *invoke(const QString &uri) noexcept;
    Q_INVOKABLE McQmlResponse *invoke(const QString &uri, const QJSValue &data) noexcept;
    Q_INVOKABLE McQmlSocket *addConnect(const QString &uri) noexcept;
    Q_INVOKABLE McQmlSocket *addConnect(const QString &uri, const QJSValue &data) noexcept;

private:
    MC_DECL_PRIVATE(McQmlRequestor)
};

MC_DECL_METATYPE(McQmlRequestor)
