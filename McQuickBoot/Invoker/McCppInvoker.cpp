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
#include "McCppInvoker.h"

#include <QJsonArray>
#include <QJsonObject>

MC_AUTO_INIT2(McCppInvoker) {}

McCppInvoker::McCppInvoker(QObject *parent) noexcept
    : McAbstractInvoker(parent)
{
}

McCppInvoker::~McCppInvoker() {}

McCppPromise &McCppInvoker::invoke(const QString &uri) noexcept
{
    auto promise = new McCppPromise(); //!< 没有指定父对象，该对象将在整个请求完毕时被析构
    run(promise, uri);
    return *promise;
}

McCppPromise &McCppInvoker::invoke(const QString &uri, const QVariant &data) noexcept
{
    return invoke(uri, QVariantList() << data);
}

McCppPromise &McCppInvoker::invoke(const QString &uri, const QVariantList &data) noexcept
{
    auto promise = new McCppPromise(); //!< 没有指定父对象，该对象将在整个请求完毕时被析构
    run(promise, uri, data);
    return *promise;
}

McCppPromise &McCppInvoker::invoke(const QString &uri, const QJsonObject &data) noexcept
{
    auto promise = new McCppPromise(); //!< 没有指定父对象，该对象将在整个请求完毕时被析构
    run(promise, uri, data.toVariantMap());
    return *promise;
}

McCppPromise &McCppInvoker::invoke(const QString &uri, const QJsonArray &data) noexcept
{
    return invoke(uri, data.toVariantList());
}

QVariant McCppInvoker::syncInvoke(const QString &uri) noexcept
{
    auto promise = new McCppPromise(); //!< 没有指定父对象，该对象将在整个请求完毕时被析构
    QVariant result;
    promise->asyncThen([&result](const QVariant &var) { result = var; });
    run(promise, uri, false);
    return result;
}

QVariant McCppInvoker::syncInvoke(const QString &uri, const QVariant &data) noexcept
{
    return syncInvoke(uri, QVariantList() << data);
}

QVariant McCppInvoker::syncInvoke(const QString &uri, const QVariantList &data) noexcept
{
    auto promise = new McCppPromise(); //!< 没有指定父对象，该对象将在整个请求完毕时被析构
    QVariant result;
    promise->asyncThen([&result](const QVariant &var) { result = var; });
    run(promise, uri, data, false);
    return result;
}

QVariant McCppInvoker::syncInvoke(const QString &uri, const QJsonObject &data) noexcept
{
    auto promise = new McCppPromise(); //!< 没有指定父对象，该对象将在整个请求完毕时被析构
    QVariant result;
    promise->asyncThen([&result](const QVariant &var) { result = var; });
    run(promise, uri, data.toVariantMap(), false);
    return result;
}

QVariant McCppInvoker::syncInvoke(const QString &uri, const QJsonArray &data) noexcept
{
    return syncInvoke(uri, data.toVariantList());
}
