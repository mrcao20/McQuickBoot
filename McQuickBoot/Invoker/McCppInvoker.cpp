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

#include "McInvoker.h"

MC_AUTO_INIT2(McCppInvoker) {}

McCppInvoker::McCppInvoker(QObject *parent) noexcept
    : McAbstractInvoker(parent)
{
}

McCppInvoker::~McCppInvoker() {}

// McCppInvoker &McCppInvoker::invoke(const QString &uri) noexcept
//{
//     auto response = new McCppInvoker();
//     run(response, uri, QVariant());
//     return *response; //!< 没有指定父对象，该对象将在整个请求完毕时被析构
// }

// McCppInvoker &McCppInvoker::invoke(const QString &uri, const QVariant &data) noexcept
//{
//     return invoke(uri, QVariantList() << data);
// }

// McCppInvoker &McCppInvoker::invoke(const QString &uri, const QVariantList &data) noexcept
//{
//     auto response = new McCppInvoker();
//     run(response, uri, data);
//     return *response; //!< 没有指定父对象，该对象将在整个请求完毕时被析构
// }

// QVariant McCppInvoker::syncInvoke(const QString &uri) noexcept
//{
//     return controllerContainer()->invoke(uri, QVariant(), McRequest());
// }

// QVariant McCppInvoker::syncInvoke(const QString &uri, const QVariant &data) noexcept
//{
//     return syncInvoke(uri, QVariantList() << data);
// }

// QVariant McCppInvoker::syncInvoke(const QString &uri, const QVariantList &data) noexcept
//{
//     return controllerContainer()->invoke(uri, data, McRequest());
// }
