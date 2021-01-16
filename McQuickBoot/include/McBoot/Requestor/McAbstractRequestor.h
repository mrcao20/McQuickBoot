#pragma once

#include <McIoc/McGlobal.h>

#include "../McBootMacroGlobal.h"

#include <QJsonObject>

#include <McIoc/Destroyer/IMcDestroyer.h>
#include <McIoc/Utils/IMcNonCopyable.h>

QT_BEGIN_NAMESPACE
class QRunable;
class QJSValue;
class QJSEngine;
QT_END_NAMESPACE

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractRequestor);

MC_FORWARD_DECL_CLASS(IMcControllerContainer);
MC_FORWARD_DECL_CLASS(McRequestorConfig);

class McAbstractResponse;

class MCQUICKBOOT_EXPORT McAbstractRequestor : public QObject,
                                               public IMcDestroyer,
                                               public IMcNonCopyable
{
    Q_OBJECT
    MC_DECL_INIT(McAbstractRequestor)
    MC_TYPELIST(IMcDestroyer)
    MC_AUTOWIRED("requestorConfig")
    Q_PRIVATE_PROPERTY(d, McRequestorConfigPtr requestorConfig MEMBER requestorConfig)
public:
    Q_INVOKABLE explicit McAbstractRequestor(QObject *parent = nullptr);
    ~McAbstractRequestor() override;

    //! 此函数什么都不做，因为此对象的所有权将转移至JavaScript，生命周期由JS管控
    void destroy() noexcept override;

    qint64 maxThreadCount() const noexcept;
    void setMaxThreadCount(int val) noexcept;
    IMcControllerContainerPtr controllerContainer() const noexcept;
    void setControllerContainer(IMcControllerContainerConstPtrRef val) noexcept;

protected:
    void customEvent(QEvent *event) override;

    void run(McAbstractResponse *response, const QString &uri, const QVariant &body) noexcept;

private:
    Q_INVOKABLE
    MC_BEAN_FINISHED
    void finished() noexcept;

private:
    MC_DECL_PRIVATE(McAbstractRequestor)
};

MC_DECL_METATYPE(McAbstractRequestor)
