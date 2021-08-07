/*
 * MIT License
 *
 * Copyright (c) 2021 mrcao20
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
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
