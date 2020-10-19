#include "McBoot/Controller/impl/McResult.h"

MC_DECL_PRIVATE_DATA(McResult)
MC_PADDING_CLANG(7)
bool isSuccess{false};
QString errMsg;
QVariant result;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McResult)
qRegisterMetaType<McResult *>("McResult");
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
