#pragma once

#include <McBoot/McBootGlobal.h>
#include <McBoot/Utils/Callback/Impl/McQmlSyncCallback.h>

#include "Object.h"

class ClassTestA : public QObject {
    Q_OBJECT
public:
    ClassTestA(QObject *parent = nullptr)
        : QObject(parent)
    {}
    Q_INVOKABLE
    MC_BEAN_FINISHED
    void end() noexcept {
        qDebug() << "ClassTestA end";
    }
};

class ClassTestB : public ClassTestA {
    Q_OBJECT
public:
    ClassTestB(QObject *parent = nullptr)
        : ClassTestA(parent)
    {}
    Q_INVOKABLE
    MC_BEAN_FINISHED
    void end() noexcept {
        qDebug() << "ClassTestB end";
    }
};

MC_DECL_METATYPE(ClassTestB)

MC_FORWARD_DECL_CLASS(McResult);

class GadgetTest2
{
    Q_GADGET
    MC_JSON_SERIALIZATION
public:
    MC_POCO_PROPERTY(QString, text2);
};
MC_DECL_METATYPE(GadgetTest2)

class GadgetTest
{
    Q_GADGET
    MC_COMPONENT
    MC_BEANNAME("gadgetTest")
    MC_JSON_SERIALIZATION
public:
    MC_POCO_PROPERTY(QString, text);
    MC_POCO_PROPERTY(QList<TmpPtr>, t);
    MC_POCO_PROPERTY(THash, tt);
    MC_POCO_PROPERTY(GadgetTest2Ptr, t2);
};
MC_DECL_METATYPE(GadgetTest)

#include <QJsonObject>
class InvokeTest : public ClassTestB
{
    Q_OBJECT
    MC_DECL_INIT(InvokeTest)
    MC_TYPELIST(ClassTestB)
    MC_CONTROLLER
    MC_BEANNAME("con")
public:
    Q_INVOKABLE explicit InvokeTest(QObject *parent = nullptr);

    Q_INVOKABLE void invoke1(const QString &aaa) noexcept;
    Q_INVOKABLE McResultPtr invoke2() noexcept;
    Q_INVOKABLE Object *invoke22() noexcept;
    Q_INVOKABLE void invoke3(const ObjectPtr &o) noexcept;
    Q_INVOKABLE QJsonObject invoke4() noexcept;
    Q_INVOKABLE GadgetTestPtr invoke5(const GadgetTestPtr &o) noexcept;
    Q_INVOKABLE GadgetTest *invoke6(GadgetTest *o) noexcept;
    Q_INVOKABLE void invoke7(IMcCallbackPtr o) noexcept;

    Q_INVOKABLE
    MC_BEAN_START
    void start() noexcept;
    
    Q_INVOKABLE
    MC_BEAN_FINISHED
    void end() noexcept;
    
    Q_INVOKABLE
    MC_THREAD_FINISHED
    void threadEnd() noexcept;
    
signals:
    void signal_1(GadgetTestPtr);

public slots:
};

MC_DECL_METATYPE(InvokeTest)
