#pragma once

#include <QObject>
#include <McBoot/McBootGlobal.h>

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

#include <McBoot/Utils/Callback/IMcCallback.h>
#include <McBoot/Utils/Callback/Impl/McCppSyncCallback.h>

#include "Param.h"

MC_FORWARD_DECL_PRIVATE_DATA(Test);

using ParamMap = QMap<QString, Param *>;

class Test : public QObject
{
    Q_OBJECT
    MC_TYPELIST();
    MC_CONTROLLER("aaa")
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
    Q_INVOKABLE QVariant vvv(const IMcCallbackPtr &callback);
    Q_INVOKABLE ParamPtr ccc();

    ParamPtr param;
    ParamMap paramMap;
    QThread *threadTest;

    void callbackTest(const ParamPtr &var) noexcept
    {
        qDebug() << "------+++++++---------" << var << var->aaa;
    }
    void callbackTest2(const QString &str, int ii) { qDebug() << "callback test2:" << str << ii; }

signals:
    void signal_sig();
    void signal_sig2();
    void signal_sig3(int);

private:
    MC_DECL_PRIVATE(Test)
};

MC_DECL_METATYPE(Test)
