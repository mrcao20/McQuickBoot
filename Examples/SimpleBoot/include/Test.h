#pragma once

#include <QObject>
#include <McBoot/McBootGlobal.h>

#include <QJsonArray>
#include <QJsonObject>

#include "Param.h"

MC_FORWARD_DECL_PRIVATE_DATA(Test);

using ParamMap = QMap<QString, Param *>;

class Test : public QObject
{
    Q_OBJECT
    MC_TYPELIST();
    MC_CONTROLLER
    MC_BEANNAME("aaa")
    MC_AUTOWIRED("param")
    Q_PROPERTY(ParamPtr param MEMBER param)
    MC_AUTOWIRED("paramMap")
    Q_PROPERTY(ParamMap paramMap MEMBER paramMap)
    MC_AUTOWIRED("t", "threadTest")
    Q_PROPERTY(QThread *t MEMBER threadTest)
public:
    Q_INVOKABLE explicit Test(QObject *parent = nullptr);
    ~Test();

    Q_INVOKABLE QJsonObject bbb(const QString &a,
                                const QJsonObject &obj,
                                const ParamPtr &p,
                                const std::function<void(int)> &func);

    Q_INVOKABLE void aaa(const std::function<void(int)> &func);
    Q_INVOKABLE ParamPtr ccc();

    ParamPtr param;
    ParamMap paramMap;
    QThread *threadTest;

signals:
    void signal_sig();
    void signal_sig2();

private:
    MC_DECL_PRIVATE(Test)
};

MC_DECL_METATYPE(Test)
