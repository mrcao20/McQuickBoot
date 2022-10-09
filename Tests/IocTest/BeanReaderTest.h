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
