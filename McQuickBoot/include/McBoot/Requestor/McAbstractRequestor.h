#pragma once

#include <McIoc/McGlobal.h>

#include "../McBootMacroGlobal.h"

#include <QJsonObject>

#include <McIoc/Utils/IMcNonCopyable.h>

QT_BEGIN_NAMESPACE
class QRunable;
class QJSValue;
class QJSEngine;
class QScxmlStateMachine;
QT_END_NAMESPACE

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractRequestor);

MC_FORWARD_DECL_CLASS(IMcControllerContainer);
MC_FORWARD_DECL_CLASS(McRequestorConfig);
MC_FORWARD_DECL_CLASS(McStateMachineConfig);

class McAbstractResponse;
class IMcApplicationContext;

class MCQUICKBOOT_EXPORT McAbstractRequestor : public QObject,
                                               public IMcNonCopyable
{
    Q_OBJECT
    MC_DECL_INIT(McAbstractRequestor)
    Q_PROPERTY(QScxmlStateMachine *stateMachine READ stateMachine NOTIFY stateMachineChanged)
    MC_AUTOWIRED("requestorConfig")
    Q_PRIVATE_PROPERTY(d, McRequestorConfigPtr requestorConfig MEMBER requestorConfig)
    MC_AUTOWIRED("stateMachineConfig")
    Q_PRIVATE_PROPERTY(d, McStateMachineConfigPtr stateMachineConfig MEMBER stateMachineConfig)
public:
    Q_INVOKABLE explicit McAbstractRequestor(QObject *parent = nullptr);
    ~McAbstractRequestor() override;

    qint64 maxThreadCount() const noexcept;
    void setMaxThreadCount(int val) noexcept;
    IMcControllerContainerPtr controllerContainer() const noexcept;
    void setControllerContainer(IMcControllerContainerConstPtrRef val) noexcept;
    IMcApplicationContext *appCtx() const noexcept;
    void setAppCtx(IMcApplicationContext *val) noexcept;

    Q_INVOKABLE QObject *getBean(const QString &name) const noexcept;
    template<typename T>
    T getBean(const QString &name) const noexcept
    {
        return getBeanToVariant(name).value<T>();
    }

    QScxmlStateMachine *stateMachine() const noexcept;
    bool isLoadStateMachine() const noexcept;

protected:
    void customEvent(QEvent *event) override;

    void run(McAbstractResponse *response, const QString &uri, const QVariant &body) noexcept;

private:
    Q_SIGNAL void stateMachineChanged();

    Q_INVOKABLE
    MC_ALL_FINISHED
    void allFinished() noexcept;

    QVariant getBeanToVariant(const QString &name) const noexcept;

private:
    MC_DECL_PRIVATE(McAbstractRequestor)
};

MC_DECL_METATYPE(McAbstractRequestor)
