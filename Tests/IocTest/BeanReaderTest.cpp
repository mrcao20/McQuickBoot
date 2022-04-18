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
    for (auto &checkKey: qAsConst(checkKeys)) {
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
    auto reader = McXmlBeanBuilderReaderPtr::create(file);
    RegistryTest registry;
    reader->readBeanBuilder(&registry);
}
