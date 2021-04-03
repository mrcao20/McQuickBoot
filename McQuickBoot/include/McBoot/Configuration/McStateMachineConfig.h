#pragma once

#include "../McBootGlobal.h"

MC_FORWARD_DECL_CLASS(QScxmlStateMachine);

MC_FORWARD_DECL_PRIVATE_DATA(McStateMachineConfig);

class McStateMachineConfig : public QObject
{
    Q_OBJECT
    MC_COMPONENT
    MC_BEANNAME("stateMachineConfig")
    MC_CONFIGURATION_PROPERTIES("boot.application.stateMachine")
    Q_PRIVATE_PROPERTY(d, QStringList paths MEMBER paths)
public:
    Q_INVOKABLE explicit McStateMachineConfig(QObject *parent = nullptr) noexcept;
    ~McStateMachineConfig() override;

    QScxmlStateMachinePtr stateMachine() const noexcept;

private:
    Q_INVOKABLE
    MC_ALL_FINISHED
    void allFinished() noexcept;

private:
    MC_DECL_PRIVATE(McStateMachineConfig)
};

MC_DECL_METATYPE(McStateMachineConfig)

