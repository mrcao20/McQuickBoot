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
MC_FORWARD_DECL_CLASS(IMcModelContainer);
MC_FORWARD_DECL_CLASS(McRequestorConfig);
MC_FORWARD_DECL_CLASS(McStateMachineConfig);
MC_FORWARD_DECL_CLASS(IMcResponseHandler);

class McAbstractResponse;
class IMcApplicationContext;

class MCQUICKBOOT_EXPORT McAbstractRequestor : public QObject,
                                               public IMcNonCopyable
{
    Q_OBJECT
    MC_DECL_INIT(McAbstractRequestor)
    Q_PROPERTY(QScxmlStateMachine *stateMachine READ stateMachine NOTIFY stateMachineChanged)
    MC_PRIVATE_PROPERTY(IMcControllerContainerPtr, controllerContainer, MEMBER controllerContainer)
    MC_PRIVATE_PROPERTY(IMcModelContainerPtr, modelContainer, MEMBER modelContainer)
    MC_PRIVATE_PROPERTY(McRequestorConfigPtr, requestorConfig, MEMBER requestorConfig)
    MC_PRIVATE_PROPERTY(McStateMachineConfigPtr, stateMachineConfig, MEMBER stateMachineConfig)
    MC_PRIVATE_PROPERTY2(QList<IMcResponseHandlerPtr>, responseHanlders, MEMBER responseHanlders)
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
    Q_INVOKABLE QObject *getModel(const QString &name) const noexcept;

    QScxmlStateMachine *stateMachine() const noexcept;
    bool isLoadStateMachine() const noexcept;
    static void setStaticStateMachine(QScxmlStateMachine *val);

protected:
    void customEvent(QEvent *event) override;

    void run(McAbstractResponse *response, const QString &uri, const QVariant &body) noexcept;
    QVariant getBeanToVariant(const QString &name) const noexcept;

private:
    Q_SIGNAL void stateMachineChanged();

    Q_INVOKABLE
    MC_ALL_FINISHED
    void allFinished() noexcept;

private:
    MC_DECL_PRIVATE(McAbstractRequestor)
};

MC_DECL_METATYPE(McAbstractRequestor)
