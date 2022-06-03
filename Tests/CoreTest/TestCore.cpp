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
#include <QDateTime>
#include <QFile>
#include <QJsonObject>
#include <QTest>
#include <QtConcurrent>

#include <McCore/Event/McEventDispatcher.h>
#include <McCore/McGlobal.h>
#include <McCore/PluginChecker/Impl/McNormalPluginChecker.h>
#include <McCore/Utils/McCancel.h>
#include <McCore/Utils/McJsonUtils.h>
#include <McCore/Utils/McPause.h>
#include <McCore/Utils/McProgress.h>

#include "MetaTypeTest.h"

MC_STATIC()
mcRegisterMetaTypeSimple<GadgetData>();
mcRegisterMetaType<ObjectData>();
mcRegisterContainer<QList<GadgetDataPtr>>();
MC_STATIC_END

void TestCore::jsonCase()
{
    auto gadget = GadgetDataPtr::create();
    auto childGadget = GadgetDataPtr::create();
    childGadget->text = "childGadget";
    gadget->child = childGadget;
    auto object = ObjectDataPtr::create();
    object->gadgets.append(gadget);
    object->gadgets.append(childGadget);
    {
        auto var = McJsonUtils::serialize(QVariant::fromValue(gadget));
        QVERIFY(var.metaType() == QMetaType::fromType<GadgetDataPtr>());
        auto tmp = var.value<GadgetDataPtr>();
        QVERIFY(!tmp.isNull());
        QVERIFY(tmp->text == QLatin1String("gadgetData"));
        QVERIFY(!tmp->child.isNull());
        QVERIFY(tmp->child->text == QLatin1String("childGadget"));
    }
    {
        auto json = McJsonUtils::toJson(gadget);
        QVERIFY(!json.isEmpty());
        QVERIFY(json.value("text") == QLatin1String("gadgetData"));
        QVERIFY(json.value("child").toObject().value("text") == QLatin1String("childGadget"));
        auto tmp = McJsonUtils::fromJson<GadgetDataPtr>(json);
        QVERIFY(!tmp.isNull());
        QVERIFY(tmp->text == QLatin1String("gadgetData"));
        QVERIFY(!tmp->child.isNull());
        QVERIFY(tmp->child->text == QLatin1String("childGadget"));
    }
    {
        auto var = McJsonUtils::serialize(QVariant::fromValue(object));
        QVERIFY(var.metaType() == QMetaType::fromType<QJsonObject>());
        QVERIFY(var.toJsonObject().value("text") == QLatin1String("objectData"));
        QVERIFY(var.toJsonObject().value("gadgets").toArray().size() == 2);
        QVERIFY(var.toJsonObject().value("gadgets").toArray().at(0).toObject().value("text")
                == QLatin1String("gadgetData"));
        QVERIFY(var.toJsonObject().value("gadgets").toArray().at(0).toObject().value("child").toObject().value("text")
                == QLatin1String("childGadget"));
        QVERIFY(var.toJsonObject().value("gadgets").toArray().at(1).toObject().value("text")
                == QLatin1String("childGadget"));
        QVERIFY(var.toJsonObject().value("gadgets").toArray().at(1).toObject().value("child").toObject().isEmpty());
        auto tmp = McJsonUtils::deserialize(var, QMetaType::fromType<ObjectDataPtr>()).value<ObjectDataPtr>();
        QVERIFY(!tmp.isNull());
        QVERIFY(tmp->text == QLatin1String("objectData"));
        QVERIFY(tmp->gadgets.size() == 2);
        QVERIFY(!tmp->gadgets.at(0).isNull());
        QVERIFY(tmp->gadgets.at(0)->text == QLatin1String("gadgetData"));
        QVERIFY(!tmp->gadgets.at(0)->child.isNull());
        QVERIFY(tmp->gadgets.at(0)->child->text == QLatin1String("childGadget"));
        QVERIFY(!tmp->gadgets.at(1).isNull());
        QVERIFY(tmp->gadgets.at(1)->text == QLatin1String("childGadget"));
    }
}

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

void TestCore::loadPluginCase()
{
#ifdef Q_OS_WIN
    QString filePath("./SimplePlugin.dll");
#else
    QString filePath("../lib/libSimplePlugin.so");
#endif
    QJsonObject json;
    json.insert("IID", "org.quickboot.mc.test.IObjectTest");
#ifdef QT_DEBUG
    json.insert("debug", true);
#endif
    json.insert("className", "SimplePlugin");
    QJsonObject metaJson;
    metaJson.insert("checkKey", "simplePlugin");
    json.insert("MetaData", metaJson);
    QVERIFY(Mc::loadPlugin(filePath, McNormalPluginChecker(json)) != nullptr);
}

void TestCore::loadLibraryCase()
{
    using TestLoadFunc = void (*)();
#ifdef Q_OS_WIN
    QString filePath("./SimpleLibrary.dll");
#else
    QString filePath("../lib/libSimpleLibrary.so");
#endif
    TestLoadFunc func = Mc::loadLibrary(Mc::toAbsolutePath(filePath), QLatin1String("testLoadFunc"), QLatin1String(""));
    QVERIFY(func != nullptr);
    func();
}

void TestCore::loadMemoryLibraryCase()
{
#ifdef Q_OS_WIN
    QString filePath("./SimpleLibrary.dll");
#else
    QString filePath("../lib/libSimpleLibrary.so");
#endif
    QFile file(filePath);
    QVERIFY(file.open(QIODevice::ReadOnly));
    auto data = file.readAll();
    using TestLoadFunc = void (*)();
    TestLoadFunc func = Mc::loadMemoryLibrary(data, QLatin1String("testLoadFunc"), QLatin1String(""));
    QVERIFY(func != nullptr);
    func();
}

void TestCore::callbackCase()
{
    McCancel cancel;
    McPause pause;
    McProgress progress;
    int current = 0, total = 0;
    progress.totalCallback([&total](int t) { total = t; });
    progress.currentCallback([&current](int c) { current = c; });

    bool isCancel = false, isPause = false, isFinished = false;
    auto future = QtConcurrent::run([&isCancel, &isPause, &isFinished, cancel, pause, progress]() {
        auto p = progress;
        while (!isCancel) {
            isCancel = cancel.isCanceled();
            isPause = pause.isPaused();
            p.setCurrent(10);
            p.setTotal(10);
        }
        isFinished = true;
    });
    auto start = QDateTime::currentMSecsSinceEpoch();
    Mc::waitForExecFunc([&isFinished, &start, cancel, pause]() {
        auto p = pause;
        auto c = cancel;
        if (QDateTime::currentMSecsSinceEpoch() - start > 500 && !pause.isPaused()) {
            p.pause();
        }
        if (QDateTime::currentMSecsSinceEpoch() - start > 1000 && !cancel.isCanceled()) {
            c.cancel();
        }
        return isFinished;
    });

    QVERIFY(current == 10);
    QVERIFY(total == 10);
    QVERIFY(isCancel);
    QVERIFY(isPause);
}
