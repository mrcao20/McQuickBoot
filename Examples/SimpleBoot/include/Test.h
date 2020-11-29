#pragma once

#include <QObject>
#include <McBoot/McBootGlobal.h>

#include <QJsonObject>

MC_FORWARD_DECL_PRIVATE_DATA(Test);

class Test : public QObject
{
    Q_OBJECT
    MC_DECL_INIT(Test)
    MC_DEFINE_TYPELIST();
    MC_CONTROLLER
    MC_BEANNAME("aaa")
public:
    Q_INVOKABLE explicit Test(QObject *parent = nullptr);
    ~Test();
    
    Q_INVOKABLE QJsonObject bbb();
    
private:
    MC_DECL_PRIVATE(Test)
};

MC_DECL_METATYPE(Test)

