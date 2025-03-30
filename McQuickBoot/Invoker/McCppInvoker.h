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
#pragma once

#include "McAbstractInvoker.h"

#include "McCppPromise.h"

class MC_QUICKBOOT_EXPORT McCppInvoker : public McAbstractInvoker
{
    Q_OBJECT
    MC_FULL_DEFINE(McCppInvoker, McAbstractInvoker)
    MC_COMPONENT("cppInvoker")
public:
    using McAbstractInvoker::getBean;
    //    using McAbstractInvoker::getModel;

    explicit McCppInvoker(QObject *parent = nullptr) noexcept;
    ~McCppInvoker() override;

    McCppPromise &invoke(const QString &uri) noexcept;
    McCppPromise &invoke(const QString &uri, const QVariant &data) noexcept;
    McCppPromise &invoke(const QString &uri, const QVariantList &data) noexcept;
    McCppPromise &invoke(const QString &uri, const QJsonObject &data) noexcept;
    McCppPromise &invoke(const QString &uri, const QJsonArray &data) noexcept;
    template<typename... Args>
    McCppPromise &invoke(const QString &uri, const Args &...args) noexcept
    {
        QVariantList vars;
        (vars << ... << McPrivate::toQVariant(args));
        return invoke(uri, vars);
    }

    QVariant syncInvoke(const QString &uri) noexcept;
    QVariant syncInvoke(const QString &uri, const QVariant &data) noexcept;
    QVariant syncInvoke(const QString &uri, const QVariantList &data) noexcept;
    QVariant syncInvoke(const QString &uri, const QJsonObject &data) noexcept;
    QVariant syncInvoke(const QString &uri, const QJsonArray &data) noexcept;
    template<typename... Args>
    QVariant syncInvoke(const QString &uri, const Args &...args) noexcept
    {
        QVariantList vars;
        (vars << ... << McPrivate::toQVariant(args));
        return syncInvoke(uri, vars);
    }

    template<typename T>
    T getBean(const QString &name) const noexcept
    {
        return McPrivate::toRealValue<T>(getBeanToVariant(name));
    }
    //    template<typename T>
    //    T getModel(const QString &name) const noexcept
    //    {
    //        QObject *obj = getModel(name);
    //        return qobject_cast<T>(obj);
    //    }
};

MC_DECL_METATYPE(McCppInvoker)
