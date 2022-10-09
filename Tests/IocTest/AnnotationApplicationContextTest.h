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

#include <McIoc/McIocGlobal.h>

struct SimpleGadget
{
    Q_GADGET
    MC_COMPONENT()
    MC_SINGLETON(false)
public:
    QString text{"simpleGadget"};
};
MC_DECL_METATYPE(SimpleGadget)

struct SimpleGadgetPointer
{
    Q_GADGET
    MC_COMPONENT("customSimpleGadget")
    MC_POINTER(true)
public:
    QString text{"simpleGadgetPointer"};
};
MC_DECL_METATYPE(SimpleGadgetPointer)

struct ComplexGadget
{
    Q_GADGET
    MC_COMPONENT()
    MC_PROPERTY(SimpleGadgetPtr, simpleGadget, MEMBER simpleGadget)
    MC_PROPERTY2(SimpleGadgetPtr, simpleGadget2, MEMBER simpleGadget2)
    MC_AUTOWIRED("simpleGadgetPointer", "customSimpleGadget")
    Q_PROPERTY(SimpleGadgetPointer *simpleGadgetPointer MEMBER simpleGadgetPointer)
    MC_RESOURCE("simpleGadgetPointer2")
    Q_PROPERTY(SimpleGadgetPointer *simpleGadgetPointer2 MEMBER simpleGadgetPointer2)
public:
    SimpleGadgetPtr simpleGadget;
    SimpleGadgetPtr simpleGadget2;
    SimpleGadgetPointer *simpleGadgetPointer{nullptr};
    SimpleGadgetPointer *simpleGadgetPointer2{nullptr};
};
MC_DECL_METATYPE(ComplexGadget)

class ObjectInterface
{
    MC_DEFINE_INTERFACE(ObjectInterface)
public:
    virtual QString test() = 0;
};
MC_DECL_METATYPE(ObjectInterface)

class ObjectPointerInterface
{
    MC_DEFINE_INTERFACE(ObjectPointerInterface)
public:
    virtual QString test() = 0;
};
MC_DECL_METATYPE(ObjectPointerInterface)

class SimpleObject
    : public QObject
    , public ObjectInterface
{
    Q_OBJECT
    MC_INTERFACES(ObjectInterface)
    MC_COMPONENT()
    MC_SINGLETON(false)
public:
    QString test() override { return m_text; }

private:
    QString m_text{"simpleObject"};
};
MC_DECL_METATYPE(SimpleObject)

class SimpleObject2
    : public QObject
    , public ObjectInterface
{
    Q_OBJECT
    MC_INTERFACES(ObjectInterface)
    MC_COMPONENT()
public:
    QString test() override { return m_text; }

private:
    QString m_text{"simpleObject2"};
};
MC_DECL_METATYPE(SimpleObject2)

class SimpleObjectPointer
    : public QObject
    , public ObjectPointerInterface
{
    Q_OBJECT
    MC_INTERFACES(ObjectPointerInterface)
    MC_COMPONENT("customSimpleObject")
    MC_POINTER(true)
public:
    QString test() override { return m_text; }

private:
    QString m_text{"simpleObjectPointer"};
};
MC_DECL_METATYPE(SimpleObjectPointer)

class SimpleObjectPointer2
    : public QObject
    , public ObjectPointerInterface
{
    Q_OBJECT
    MC_INTERFACES(ObjectPointerInterface)
    MC_COMPONENT("customSimpleObject2")
    MC_POINTER(true)
public:
    QString test() override { return m_text; }

private:
    QString m_text{"simpleObjectPointer2"};
};
MC_DECL_METATYPE(SimpleObjectPointer2)

class ComplexObject : public QObject
{
    Q_OBJECT
    MC_COMPONENT()
    MC_PROPERTY(ObjectInterfacePtr, simpleObject, MEMBER simpleObject)
    MC_PROPERTY2(SimpleObject2Ptr, simpleObject2, MEMBER simpleObject2)
    MC_PROPERTY(ObjectPointerInterface *, customSimpleObject, MEMBER customSimpleObject)
    MC_PROPERTY2(SimpleObjectPointer2 *, customSimpleObject2, MEMBER customSimpleObject2)
    MC_PROPERTY2(QList<ObjectInterfacePtr>, objectInterfaceList, MEMBER objectInterfaceList)
    MC_RESOURCE("objectInterfaceMap")
    Q_PROPERTY(QMap<QString, ObjectInterfacePtr> objectInterfaceMap MEMBER objectInterfaceMap)
    MC_PROPERTY2(QList<ObjectPointerInterface *>, objectPointerInterfaceList, MEMBER objectPointerInterfaceList)
    MC_RESOURCE("objectPointerInterfaceMap")
    Q_PROPERTY(QMap<QString, ObjectPointerInterface *> objectPointerInterfaceMap MEMBER objectPointerInterfaceMap)
public:
    ObjectInterfacePtr simpleObject;
    SimpleObject2Ptr simpleObject2;
    ObjectPointerInterface *customSimpleObject{nullptr};
    SimpleObjectPointer2 *customSimpleObject2{nullptr};
    QList<ObjectInterfacePtr> objectInterfaceList;
    QMap<QString, ObjectInterfacePtr> objectInterfaceMap;
    QList<ObjectPointerInterface *> objectPointerInterfaceList;
    QMap<QString, ObjectPointerInterface *> objectPointerInterfaceMap;
};
MC_DECL_METATYPE(ComplexObject)

//////////////////////////////////////////////////////////////////
MC_FORWARD_DECL_CLASS(IMcApplicationContext)

class AnnotationApplicationContextTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void gadgetCase();
    void objectCase();

private:
    IMcApplicationContextPtr m_appCtx;
};
