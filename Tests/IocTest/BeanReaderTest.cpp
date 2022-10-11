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
#include "BeanReaderTest.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>

#include <McIoc/BeanBuilderReader/Impl/McXmlBeanBuilderReader.h>

MC_AUTO_INIT(PluginCheckerTest)
MC_INIT_END

bool PluginCheckerTest::check(const QJsonObject &json) noexcept
{
    auto checkJson = QJsonDocument::fromJson(
        R"({"IID": "org.quickboot.mc.test.IObjectTest", "className": "SimplePlugin", "MetaData": {"checkKey": "simplePlugin"}})")
                         .object();
    auto checkKeys = checkJson.keys();
    for (auto &checkKey : qAsConst(checkKeys)) {
        if (!json.contains(checkKey)) {
            return false;
        }
        auto checkValue = checkJson.value(checkKey);
        auto value = json.value(checkKey);
        if (checkValue != value) {
            return false;
        }
    }
    return true;
}

bool RegistryTest::registerBeanBuilder(const QString &name, const IMcBeanBuilderPtr &beanBuilder) noexcept
{
    m_registerBeanNames.removeAll(name);
    if (m_hash.contains(name)) {
        qWarning("'%s' is already registered. replaced", qPrintable(name));
    }
    m_hash.insert(name, beanBuilder);
    return true;
}

bool RegistryTest::registerBeanBuilder(const QHash<QString, IMcBeanBuilderPtr> &vals) noexcept
{
    bool ret = true;
    QHashIterator<QString, IMcBeanBuilderPtr> itr(vals);
    while (itr.hasNext()) {
        auto item = itr.next();
        if (!registerBeanBuilder(item.key(), item.value())) {
            ret = false;
        }
    }
    return ret;
}

IMcBeanBuilderPtr RegistryTest::unregisterBeanBuilder(const QString &name) noexcept
{
    return m_hash.take(name);
}

bool RegistryTest::isContained(const QString &name) const noexcept
{
    return m_hash.contains(name);
}

QHash<QString, IMcBeanBuilderPtr> RegistryTest::getBeanBuilders() const noexcept
{
    return m_hash;
}

void BeanReaderTest::readerCase()
{
    auto file = QSharedPointer<QFile>::create((":/iocTest.xml"));
    QVERIFY(file->open(QIODevice::ReadOnly));
    {
        QList<QString> beanNames{"podTest", "podTestPointer", "gadgetTest", "gadgetTestPointer", "listTest", "mapTest",
            "objectTest", "objectTestPointer"};
        auto reader = McXmlBeanBuilderReaderPtr::create(file);
        RegistryTest registry(beanNames);
        reader->readBeanBuilder(&registry);
        QVERIFY(beanNames.isEmpty());
    }
    {
        QList<QString> beanNames{"podTest", "podTestPointer", "gadgetTest", "gadgetTestPointer", "listTest", "mapTest",
            "objectTest", "objectTestPointer", "pluginTest", "pluginTestPointer"};
#ifdef Q_OS_WIN
        auto reader = McXmlBeanBuilderReaderPtr::create(file, "debug");
#else
        auto reader = McXmlBeanBuilderReaderPtr::create(file, "linuxDebug");
#endif
        RegistryTest registry(beanNames);
        reader->readBeanBuilder(&registry);
        QVERIFY(beanNames.isEmpty());
    }
}
