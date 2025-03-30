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
