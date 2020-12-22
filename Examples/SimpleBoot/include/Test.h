#pragma once

#include <QObject>
#include <McBoot/McBootGlobal.h>

#include <QJsonArray>
#include <QJsonObject>

#include "Param.h"

MC_FORWARD_DECL_PRIVATE_DATA(Test);

class Test : public QObject
{
    Q_OBJECT
    MC_DEFINE_TYPELIST();
    MC_CONTROLLER
    MC_BEANNAME("aaa")
public:
    Q_INVOKABLE explicit Test(QObject *parent = nullptr);
    ~Test();

    Q_INVOKABLE QJsonObject bbb(const QString &a,
                                const QJsonObject &obj,
                                const ParamPtr &p,
                                const std::function<void(int)> &func);

    Q_INVOKABLE void aaa(const std::function<void(int)> &func);

private:
    MC_DECL_PRIVATE(Test)
};

MC_DECL_METATYPE(Test)

