#include "McBoot/Controller/impl/McQmlResponse.h"

#include <QCoreApplication>
#include <QVariant>
#include <QJSValue>
#include <QJSEngine>
#include <QEvent>

#include <McIoc/Utils/McScopedFunction.h>

MC_DECL_PRIVATE_DATA(McQmlResponse)
MC_PADDING_CLANG(7)
bool isSyncCall{false};         // 是否让线程回到主线程再执行callback，默认不需要
QJSValue callback;
QVariant body;
QJSEngine *jsEngine{nullptr};
MC_DECL_PRIVATE_DATA_END

MC_INIT(McQmlResponse)
qRegisterMetaType<McQmlResponse *>("McQmlResponse");
MC_INIT_END

McQmlResponse::McQmlResponse(QJSEngine *jsEngine, QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McQmlResponse);
    
    d->jsEngine = jsEngine;
}

McQmlResponse::~McQmlResponse() 
{
}

void McQmlResponse::setBody(const QVariant &var) noexcept 
{
    d->body = var;
    
    if(d->isSyncCall) {
        //! 发布的事件由QT删除
        qApp->postEvent(this, new QEvent(static_cast<QEvent::Type>(QEvent::Type::User + 1)));
        return;
    }
    
    callCallback();
}

McQmlResponse *McQmlResponse::then(const QJSValue &callback) noexcept 
{
    d->callback = callback;
    d->isSyncCall = false;
    return this;
}

McQmlResponse *McQmlResponse::syncThen(const QJSValue &callback) noexcept 
{
    d->callback = callback;
    d->isSyncCall = true;
    return this;
}

void McQmlResponse::customEvent(QEvent *event) 
{
    if(event->type() == QEvent::Type::User + 1) {
        callCallback();
    }
}

void McQmlResponse::callCallback() noexcept 
{
    McScopedFunction func([this](){
        this->deleteLater();
    });
    if(!d->callback.isCallable()) {
        return;
    }
    auto engine = d->jsEngine;
    if(!engine) {
        return;
    }
    auto arg = engine->toScriptValue(d->body);
    d->callback.call(QJSValueList() << arg);
}
