#pragma once

#include <McBootGlobal.h>

#include "Object.h"

class McResult;

class InvokeTest : public QObject
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
    
signals:
    
public slots:
};

MC_DECL_METATYPE(InvokeTest)
