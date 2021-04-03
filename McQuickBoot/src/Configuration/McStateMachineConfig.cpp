#include "McBoot/Configuration/McStateMachineConfig.h"

#include <QScxmlStateMachine>

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(McStateMachineConfig)
MC_STATIC_END

MC_DECL_PRIVATE_DATA(McStateMachineConfig)
QStringList paths;
QScxmlStateMachinePtr stateMachine;
MC_DECL_PRIVATE_DATA_END

McStateMachineConfig::McStateMachineConfig(QObject *parent) noexcept
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McStateMachineConfig);
}

McStateMachineConfig::~McStateMachineConfig()
{
}

QScxmlStateMachinePtr McStateMachineConfig::stateMachine() const noexcept
{
    return d->stateMachine;
}

void McStateMachineConfig::allFinished() noexcept
{
    if (d->paths.length() != 1) {
        return;
    }
    d->stateMachine.reset(QScxmlStateMachine::fromFile(Mc::toAbsolutePath(d->paths.first())));
    //! 这里可以在状态机构造完成后直接start，但它不会立即执行，它会等待至少一个事件循环开始后才会执行
    //! 所以在此处start不会造成状态异常
    d->stateMachine->start();
}

#include "moc_McStateMachineConfig.cpp"
