#include "McBoot/Controller/impl/McResult.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

MC_DECL_PRIVATE_DATA(McResult)
bool isInternalError{false};
bool isSuccess{false};
QString errMsg;
QVariant result;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McResult)
MC_REGISTER_BEAN_FACTORY(McResult);
QMetaType::registerDebugStreamOperator<McResult *>();
QMetaType::registerDebugStreamOperator<McResultPtr>();
MC_INIT_END

McResult::McResult(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McResult)
}

McResult::~McResult() 
{
}

bool McResult::isSuccess() const noexcept 
{
    return d->isSuccess;
}

void McResult::setSuccess(bool val) noexcept 
{
    d->isSuccess = val;
}

QString McResult::errMsg() const noexcept 
{
    return d->errMsg;
}

void McResult::setErrMsg(const QString &val) noexcept 
{
    d->errMsg = val;
}

QVariant McResult::result() const noexcept 
{
    return d->result;
}

void McResult::setResult(const QVariant &val) noexcept 
{
    d->result = val;
}

QDebug operator<<(QDebug dbg, McResult *r)
{
    QDebugStateSaver saver(dbg);
    QJsonObject jsonObj;
    jsonObj.insert("isSuccess", r->isSuccess());
    jsonObj.insert("result", QJsonValue::fromVariant(r->result()));
    jsonObj.insert("errMsg", r->errMsg());
    dbg.noquote() << QJsonDocument(jsonObj).toJson();
    return dbg;
}

QDebug operator<<(QDebug dbg, const QSharedPointer<McResult> &r)
{
    QDebugStateSaver saver(dbg);
    QJsonObject jsonObj;
    jsonObj.insert("isSuccess", r->isSuccess());
    jsonObj.insert("result", QJsonValue::fromVariant(r->result()));
    jsonObj.insert("errMsg", r->errMsg());
    dbg.noquote() << QJsonDocument(jsonObj).toJson();
    return dbg;
}

bool McResult::isInternalError() const noexcept
{
    return d->isInternalError;
}

void McResult::setInternalError(bool val) noexcept
{
    d->isInternalError = val;
}
