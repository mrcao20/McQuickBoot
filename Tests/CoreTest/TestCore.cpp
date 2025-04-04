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
#include <McCore/Utils/McRequest.h>

#include "MetaTypeTest.h"

MC_STATIC()
mcRegisterMetaTypeSimple<GadgetData>();
mcRegisterMetaType<ObjectData>();
mcRegisterContainer<QList<GadgetDataPtr>>();
MC_STATIC_END

void TestCore::coreCase()
{
    int a = 1;
    QString b = QStringLiteral("b");
    GadgetDataPtr c = GadgetDataPtr::create();
    McRequest request;
    request.addParam(a);
    request.addParam(b);
    request.addParam(c);
    QVERIFY(request.rollAt<int>() == a);
    QVERIFY(request.rollAt<QString>() == b);
    QVERIFY(request.rollAt<GadgetDataPtr>() == c);
    QVERIFY(request.rollAt<QString>() != b);
    QVERIFY(request.rollAt<GadgetDataPtr>() != c);
    QVERIFY(request.rollAt<int>() != a);
}

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
#ifdef MC_USE_QT5
        QVERIFY(var.userType() == qMetaTypeId<GadgetDataPtr>());
#else
        QVERIFY(var.metaType() == QMetaType::fromType<GadgetDataPtr>());
#endif
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
#ifdef MC_USE_QT5
        QVERIFY(var.userType() == qMetaTypeId<QJsonObject>());
#else
        QVERIFY(var.metaType() == QMetaType::fromType<QJsonObject>());
#endif
        QVERIFY(var.toJsonObject().value("text") == QLatin1String("objectData"));
        QVERIFY(var.toJsonObject().value("gadgets").toArray().size() == 2);
        QVERIFY(var.toJsonObject().value("gadgets").toArray().at(0).toObject().value("text")
                == QLatin1String("gadgetData"));
        QVERIFY(var.toJsonObject().value("gadgets").toArray().at(0).toObject().value("child").toObject().value("text")
                == QLatin1String("childGadget"));
        QVERIFY(var.toJsonObject().value("gadgets").toArray().at(1).toObject().value("text")
                == QLatin1String("childGadget"));
        QVERIFY(var.toJsonObject().value("gadgets").toArray().at(1).toObject().value("child").toObject().isEmpty());
#ifdef MC_USE_QT5
        auto tmp = McJsonUtils::deserialize(var, qMetaTypeId<ObjectDataPtr>()).value<ObjectDataPtr>();
#else
        auto tmp = McJsonUtils::deserialize(var, QMetaType::fromType<ObjectDataPtr>()).value<ObjectDataPtr>();
#endif
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
    int result2 = -1;
    Mc::eventDispatcher().connectToEvent("event", [&result, &result2](const QVariant &var, int i) {
        result = var.toInt();
        result2 = i;
    });
    Mc::eventDispatcher().submitEvent("event", 1, 2);
    QVERIFY(result == 1);
    QVERIFY(result2 == 2);
    Mc::eventDispatcher().submitEvent("event", "ff", 3);
    QVERIFY(result != 1);
    QVERIFY(result2 == 3);
}

void TestCore::metaTypeCase()
{
    auto metaType = McMetaType::fromType<MetaTypeTest>();
    auto star = metaType.createPointer();
    QVERIFY(star != nullptr);
    QVariant var(metaType.pMetaType(), &star);
    auto test = var.value<MetaTypeTest *>();
    QVERIFY(test != nullptr);
    auto itest = var.value<IMetaTypeTest *>();
    QVERIFY(itest != nullptr);

#ifdef MC_USE_QT5
    auto metaObj = QMetaType::metaObjectForType(metaType.pMetaType());
#else
    auto metaObj = metaType.pMetaType().metaObject();
#endif
    QVERIFY(metaObj != nullptr);
    auto index = metaObj->indexOfMethod("testFunc(IMetaTypeTestPtr)");
    QVERIFY(index != -1);
    auto me = metaObj->method(index);
#ifdef MC_USE_QT5
    QVERIFY(me.returnType() == qMetaTypeId<IMetaTypeTestPtr>());
    QVERIFY(me.parameterType(0) == qMetaTypeId<IMetaTypeTestPtr>());
#else
    QVERIFY(me.returnMetaType() == QMetaType::fromType<IMetaTypeTestPtr>());
    QVERIFY(me.parameterMetaType(0) == QMetaType::fromType<IMetaTypeTestPtr>());
#endif
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
    progress.onTotalCallback([&total](int t) { total = t; });
    progress.onCurrentCallback(this, [&current](int c) { current = c; });

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

    auto functor = McSlotObjectWrapper::build(nullptr, []() { return 1; });
    QVERIFY(current == 10);
    QVERIFY(total == 10);
    QVERIFY(isCancel);
    QVERIFY(isPause);
    QVERIFY(functor.call(QVariantList()).toInt() == 1);
}
