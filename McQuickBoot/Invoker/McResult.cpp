/*
 * Copyright (c) 2021 mrcao20/mrcao20@163.com
 * McQuickBoot is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#include "McResult.h"

#include <QJsonDocument>
#include <QJsonObject>

MC_AUTO_INIT2(McResult)
{
#ifdef MC_USE_QT5
    QMetaType::registerDebugStreamOperator<McResult *>();
    QMetaType::registerDebugStreamOperator<McResultPtr>();
#endif
}

MC_DECL_PRIVATE_DATA2(McResult)
{
    bool isInternalError{false};
    bool isSuccess{false};
    QString errMsg;
    QVariant result;
};

McResult::McResult(QObject *parent) noexcept
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McResult);
}

McResult::~McResult() {}

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

bool McResult::isInternalError() const noexcept
{
    return d->isInternalError;
}

void McResult::setInternalError(bool val) noexcept
{
    d->isInternalError = val;
}

QDebug operator<<(QDebug dbg, const McResult &r)
{
    QDebugStateSaver saver(dbg);
    QJsonObject jsonObj;
    jsonObj.insert("isSuccess", r.isSuccess());
    jsonObj.insert("result", QJsonValue::fromVariant(r.result()));
    jsonObj.insert("errMsg", r.errMsg());
    dbg.noquote() << QJsonDocument(jsonObj).toJson();
    return dbg;
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
