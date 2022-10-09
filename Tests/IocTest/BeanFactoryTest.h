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
