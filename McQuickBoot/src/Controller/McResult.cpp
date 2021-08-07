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
