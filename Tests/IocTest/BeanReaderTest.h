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

#include <QObject>

#include <McCore/PluginChecker/IMcPluginChecker.h>
#include <McIoc/BeanFactory/IMcBeanBuilderRegistry.h>

class PluginCheckerTest : public IMcPluginChecker
{
    MC_FULL_DEFINE(PluginCheckerTest, IMcPluginChecker)
public:
    bool check(const QJsonObject &json) noexcept override;
};

MC_DECL_METATYPE(PluginCheckerTest)

class RegistryTest : public IMcBeanBuilderRegistry
{
public:
    RegistryTest(QList<QString> &val)
        : m_registerBeanNames(val)
    {
    }

    bool registerBeanBuilder(const QString &name, const IMcBeanBuilderPtr &beanBuilder) noexcept override;
    bool registerBeanBuilder(const QHash<QString, IMcBeanBuilderPtr> &vals) noexcept override;
    IMcBeanBuilderPtr unregisterBeanBuilder(const QString &name) noexcept override;
    bool isContained(const QString &name) const noexcept override;
    QHash<QString, IMcBeanBuilderPtr> getBeanBuilders() const noexcept override;

private:
    QList<QString> &m_registerBeanNames;
    QHash<QString, IMcBeanBuilderPtr> m_hash;
};

class BeanReaderTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void readerCase();
};
