#pragma once

#include <McBootGlobal.h>

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

class McResult;

class InvokeTest : public ClassTestB
{
    Q_OBJECT
    MC_DECL_INIT(InvokeTest)
    Q_CLASSINFO(MC_COMPONENT, MC_CONTROLLER)
    Q_CLASSINFO(MC_BEANNAME, "con")
public:
    Q_INVOKABLE explicit InvokeTest(QObject *parent = nullptr);
    
    Q_INVOKABLE void invoke1() noexcept;
    Q_INVOKABLE McResult *invoke2() noexcept;
    Q_INVOKABLE void invoke3(const ObjectPtr &o) noexcept;
    
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
    
public slots:
};

MC_DECL_METATYPE(InvokeTest)
