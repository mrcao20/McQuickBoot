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
#include "TestCore.h"

#include <QCoreApplication>
#include <QFile>
#include <QTest>

#include <McCore/Event/McEventDispatcher.h>
#include <McCore/McGlobal.h>

#include "MetaTypeTest.h"

void TestCore::pathPlaceholderCase()
{
    Mc::registerPathPlaceholder("{test}", []() { return QCoreApplication::applicationDirPath(); });
    QVERIFY(QDir::toNativeSeparators(Mc::toAbsolutePath("{test}"))
            == QDir::toNativeSeparators(QCoreApplication::applicationDirPath()));
}

void TestCore::eventDispatcherCase()
{
    int result = -1;
    Mc::eventDispatcher().connectToEvent("event", [&result](const QVariant &var) { result = var.toInt(); });
    Mc::eventDispatcher().submitEvent("event", 1);
    QVERIFY(result == 1);
    Mc::eventDispatcher().submitEvent("event", "ff");
    QVERIFY(result != 1);
}

void TestCore::metaTypeCase()
{
    auto metaType = McMetaType::fromType<MetaTypeTest>();
    auto star = metaType.metaType().create();
    QVERIFY(star != nullptr);
    QVariant var(metaType.pMetaType(), &star);
    auto test = var.value<MetaTypeTest *>();
    QVERIFY(test != nullptr);
    auto itest = var.value<IMetaTypeTest *>();
    QVERIFY(itest != nullptr);

    auto metaObj = metaType.pMetaType().metaObject();
    QVERIFY(metaObj != nullptr);
    auto index = metaObj->indexOfMethod("testFunc(IMetaTypeTestPtr)");
    QVERIFY(index != -1);
    auto me = metaObj->method(index);
    QVERIFY(me.returnMetaType() == QMetaType::fromType<IMetaTypeTestPtr>());
    QVERIFY(me.parameterMetaType(0) == QMetaType::fromType<IMetaTypeTestPtr>());
}

void TestCore::loadLibraryCase()
{
    using TestLoadFunc = void (*)();
#ifdef QT_DEBUG
#ifdef Q_CC_GNU
#ifdef Q_OS_UNIX
    QString fileName = "../lib/libSimpleLibraryd.so";
#else
    QString fileName = "./libSimpleLibraryd.dll";
#endif
#else
    QString fileName = "./SimpleLibraryd.dll";
#endif
#else
#ifdef Q_CC_GNU
#ifdef Q_OS_UNIX
    QString fileName = "../lib/libSimpleLibrary.so";
#else
    QString fileName = "./libSimpleLibrary.dll";
#endif
#else
    QString fileName = "./SimpleLibrary.dll";
#endif
#endif
    TestLoadFunc func = Mc::loadLibrary(Mc::toAbsolutePath(fileName), QLatin1String("testLoadFunc"), QLatin1String(""));
    QVERIFY(func != nullptr);
    func();
}

void TestCore::loadMemoryLibraryCase()
{
#ifdef QT_DEBUG
#ifdef Q_CC_GNU
#ifdef Q_OS_UNIX
    QString fileName = "../lib/libSimpleLibraryd.so";
#else
    QString fileName = "./libSimpleLibraryd.dll";
#endif
#else
    QString fileName = "./SimpleLibraryd.dll";
#endif
#else
#ifdef Q_CC_GNU
#ifdef Q_OS_UNIX
    QString fileName = "../lib/libSimpleLibrary.so";
#else
    QString fileName = "./libSimpleLibrary.dll";
#endif
#else
    QString fileName = "./SimpleLibrary.dll";
#endif
#endif
    QFile file(fileName);
    QVERIFY(file.open(QIODevice::ReadOnly));
    auto data = file.readAll();
    using TestLoadFunc = void (*)();
    TestLoadFunc func = Mc::loadMemoryLibrary(data, QLatin1String("testLoadFunc"), QLatin1String(""));
    QVERIFY(func != nullptr);
    func();
}
