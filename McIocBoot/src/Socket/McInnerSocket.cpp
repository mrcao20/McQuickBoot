#include "McBoot/Socket/impl/McInnerSocket.h"

#include <QMetaMethod>
#include <QVector>

MC_DECL_PRIVATE_DATA(McInnerSocket)
QObjectPtr innerSocketObj;
QMetaMethod onOpen;
QMetaMethod onClose;
QMetaMethod onError;
QMetaMethod onMessage;
MC_DECL_PRIVATE_DATA_END

McInnerSocket::McInnerSocket(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McInnerSocket)
}

McInnerSocket::~McInnerSocket() 
{
}

void McInnerSocket::setInnerSocketObj(QObjectConstPtrRef obj) noexcept 
{
    d->innerSocketObj = obj;
}

void McInnerSocket::setOnOpen(const QMetaMethod &method) noexcept 
{
    d->onOpen = method;
}

void McInnerSocket::setOnClose(const QMetaMethod &method) noexcept 
{
    d->onClose = method;
}

void McInnerSocket::setOnError(const QMetaMethod &method) noexcept 
{
    d->onError = method;
}

void McInnerSocket::setOnMessage(const QMetaMethod &method) noexcept 
{
    d->onMessage = method;
}

void McInnerSocket::opened(IMcSessionConstPtrRef session) noexcept 
{
    auto method = d->onOpen;
    
    if(!method.isValid()) {
        return;
    }
    
    QVector<QGenericArgument> arguments;
	arguments.resize(10);
    
    int sessionTypeId = QMetaType::type(MC_MACRO_STR(IMcSessionPtr));
    for(int i = 0; i < method.parameterCount(); ++i) {
        if(i >= 10) {
            break;
        }
        auto id = method.parameterType(i);
        if(id == sessionTypeId) {
            arguments.replace(i, Q_ARG(IMcSessionPtr, session));
            break;
        }
    }
    
    invoke(method, arguments);
}

void McInnerSocket::closed(IMcSessionConstPtrRef session) noexcept 
{
    auto method = d->onClose;
    
    if(!method.isValid()) {
        return;
    }
    
    QVector<QGenericArgument> arguments;
	arguments.resize(10);
    
    int sessionTypeId = QMetaType::type(MC_MACRO_STR(IMcSessionPtr));
    for(int i = 0; i < method.parameterCount(); ++i) {
        if(i >= 10) {
            break;
        }
        auto id = method.parameterType(i);
        if(id == sessionTypeId) {
            arguments.replace(i, Q_ARG(IMcSessionPtr, session));
            break;
        }
    }
    
    invoke(method, arguments);
}

void McInnerSocket::errored(IMcSessionConstPtrRef session, const QString &errMsg) noexcept 
{
    auto method = d->onError;
    
    if(!method.isValid()) {
        return;
    }
    
    QVector<QGenericArgument> arguments;
	arguments.resize(10);
    
    int sessionTypeId = QMetaType::type(MC_MACRO_STR(IMcSessionPtr));
    int stringTypeId = QMetaType::type(MC_MACRO_STR(QString));
    for(int i = 0; i < method.parameterCount(); ++i) {
        if(i >= 10) {
            break;
        }
        auto id = method.parameterType(i);
        if(id == sessionTypeId) {
            arguments.replace(i, Q_ARG(IMcSessionPtr, session));
        }else if(id == stringTypeId) {
            arguments.replace(i, Q_ARG(QString, errMsg));
        }
    }
    
    invoke(method, arguments);
}

void McInnerSocket::receivedMsg(IMcSessionConstPtrRef session, const QVariant &msg) noexcept 
{
    auto method = d->onMessage;
    
    if(!method.isValid()) {
        return;
    }
    
    auto tempMsg = msg;
    
    QVector<QGenericArgument> arguments;
	arguments.resize(10);
    
    int sessionTypeId = QMetaType::type(MC_MACRO_STR(IMcSessionPtr));
    for(int i = 0; i < method.parameterCount(); ++i) {
        if(i >= 10) {
            break;
        }
        auto id = method.parameterType(i);
        Q_ASSERT_X(id != QMetaType::Type::UnknownType
                , "InnerSocket"
                , "onMessage contained a not register type");
        if(id == sessionTypeId) {
            arguments.replace(i, Q_ARG(IMcSessionPtr, session));
        }else{
            tempMsg.convert(id);
            arguments.replace(i, QGenericArgument("QVariant", tempMsg.data()));
        }
    }
    
    invoke(method, arguments);
}

void McInnerSocket::invoke(const QMetaMethod &method
                           , const QVector<QGenericArgument> &args) noexcept 
{
    method.invoke(d->innerSocketObj.data(),
            Qt::DirectConnection, 
            args.at(0),
            args.at(1),
            args.at(2),
            args.at(3),
            args.at(4),
            args.at(5),
            args.at(6),
            args.at(7),
            args.at(8),
            args.at(9));
}
