#pragma once

#include "../McBootGlobal.h"

#include <QJsonObject>

#include <McIoc/Destroyer/IMcDestroyer.h>

QT_BEGIN_NAMESPACE
class QRunable;
class QJSValue;
class QJSEngine;
QT_END_NAMESPACE

MC_FORWARD_DECL_PRIVATE_DATA(McQmlRequestor);

MC_FORWARD_DECL_CLASS(IMcControllerContainer);
MC_FORWARD_DECL_CLASS(IMcQmlSocketContainer);
MC_FORWARD_DECL_CLASS(McQmlRequestorConfig);

class McQmlResponse;
class McQmlSocket;

class MCQUICKBOOT_EXPORT McQmlRequestor : public QObject, public IMcDestroyer
{
    Q_OBJECT
    MC_DECL_INIT(McQmlRequestor)
    MC_DEFINE_TYPELIST(IMcDestroyer)
    MC_COMPONENT
    MC_BEANNAME("requestor")
    //! 在容器中为非单例，但是McQuickBoot会控制其为单例
    MC_SINGLETON(false)
    MC_AUTOWIRED("requestorConfig")
    Q_PRIVATE_PROPERTY(d, McQmlRequestorConfigPtr requestorConfig MEMBER requestorConfig)
public:
    Q_INVOKABLE explicit McQmlRequestor(QObject *parent = nullptr);
    ~McQmlRequestor() override;
    
    //! 此函数什么都不做，因为此对象的所有权将转移至JavaScript，生命周期由JS管控
    void destroy() noexcept override;

    qint64 maxThreadCount() const noexcept;
    void setMaxThreadCount(int val) noexcept;
    void setControllerContainer(IMcControllerContainerConstPtrRef val) noexcept;
    void setSocketContainer(IMcQmlSocketContainerConstPtrRef val) noexcept;
    
    Q_INVOKABLE McQmlResponse *invoke(const QString &uri) noexcept;
	Q_INVOKABLE McQmlResponse *invoke(const QString &uri, const QJsonObject &data) noexcept;
    Q_INVOKABLE QVariant syncInvoke(const QString &uri) noexcept;
    Q_INVOKABLE QVariant syncInvoke(const QString &uri, const QJsonObject &data) noexcept;
    Q_INVOKABLE McQmlSocket *addConnect(const QString &uri) noexcept;
    Q_INVOKABLE McQmlSocket *addConnect(const QString &uri, const QJSValue &data) noexcept;
    
protected:
    void customEvent(QEvent *event) override;
    
private:
    Q_INVOKABLE
    MC_BEAN_FINISHED
    void finished() noexcept;
    
private:
    void run(McQmlResponse *response, const QString &uri, const QVariant &body) noexcept;
    
private:
    MC_DECL_PRIVATE(McQmlRequestor)
};

MC_DECL_METATYPE(McQmlRequestor)
