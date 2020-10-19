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
MC_DECL_PRIVATE_DATA_END

MC_INIT(McQmlResponse)
qRegisterMetaType<McQmlResponse *>("McQmlResponse");
MC_INIT_END

McQmlResponse::McQmlResponse(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McQmlResponse);
}

McQmlResponse::~McQmlResponse()
{
}

void McQmlResponse::setBody(const QVariant &var) noexcept 
{
    //! 2020-9-18
    //! 由于js为单线程语言，只能在创建该对象的线程中调用回调函数
    d->isSyncCall = true;
    
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
    Q_UNUSED(func)
    if(!d->callback.isCallable()) {
        return;
    }
#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
    auto engine = QQmlEngine::contextForObject(this)->engine();
#else
    auto engine = qjsEngine(this);
#endif
    if(!engine) {
        return;
    }
    auto arg = engine->toScriptValue(d->body);
    d->callback.call(QJSValueList() << arg);
}
