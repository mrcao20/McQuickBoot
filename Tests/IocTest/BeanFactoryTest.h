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

#include <McCore/McGlobal.h>

#include "IObjectTest.h"

struct PodTest
{
    QString text{"podTest"};
};
MC_DECL_METATYPE(PodTest)

struct GadgetTest : public IMcDestroyer
{
    Q_GADGET
    Q_PROPERTY(QString text MEMBER text)
public:
    GadgetTest() {}
    Q_INVOKABLE GadgetTest(const QString &val)
        : text(val)
    {
    }

    void destroy() override { delete this; }

    QString text{"gadgetTest"};
};
MC_DECL_METATYPE(GadgetTest)

class ObjectTest
    : public QObject
    , public IObjectTest
    , public IMcDestroyer
{
    Q_OBJECT
    Q_INTERFACES(IObjectTest)
    MC_FULL_DEFINE(ObjectTest, QObject, IObjectTest, IMcDestroyer)
    Q_PROPERTY(QString text MEMBER m_text)
    Q_PROPERTY(ObjectTestPtr object MEMBER m_object)
    Q_PROPERTY(QStringList list MEMBER m_list)
    Q_PROPERTY(QMap<QString, QSharedPointer<ObjectTest>> map MEMBER m_map)
public:
    ObjectTest();
    Q_INVOKABLE ObjectTest(const QString &val);

    void destroy() override { deleteLater(); }

    QString test() override;

Q_SIGNALS:
    void testSignal();

private Q_SLOTS:
    void testSlot();

private:
    QString m_text{"objectTest"};
    QSharedPointer<ObjectTest> m_object;
    QStringList m_list;
    QMap<QString, QSharedPointer<ObjectTest>> m_map;
};
MC_DECL_METATYPE(ObjectTest)

class BeanFactoryTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void gadgetTestCase();
    void sharedGadgetTestCase();
    void objectTestCase();
    void sharedObjectTestCase();
    void pluginTestCase();
    void sharedPluginTestCase();
    void listTestCase();
    void mapTestCase();
};
