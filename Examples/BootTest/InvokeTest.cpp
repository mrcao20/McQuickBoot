#include "InvokeTest.h"

#include <QThread>
#include <QDebug>

#include <McIoc/McGlobal.h>
#include <McBoot/Controller/impl/McResult.h>
#include <McBoot./Utils/McJsonUtils.h>
#include <McBoot/McQuickBoot.h>
#include <McBoot/Controller/impl/McQmlResponse.h>

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(GadgetTest)
MC_REGISTER_BEAN_FACTORY(GadgetTest2)
MC_STATIC_END

MC_INIT(InvokeTest)
MC_REGISTER_BEAN_FACTORY(InvokeTest)
MC_INIT_END

InvokeTest::InvokeTest(QObject *parent) 
    : ClassTestB(parent)
{
}

void InvokeTest::invoke1(const QString &aaa) noexcept
{
    qDebug() << "invoke1" << aaa;
}

McResultPtr InvokeTest::invoke2() noexcept
{
    static quint64 i = 0;
    qDebug() << "-----------" << i++;
    auto obj = McResult::fail("");
    return obj;
}

Object *InvokeTest::invoke22() noexcept
{
    return new Object();
}

void InvokeTest::invoke3(const ObjectPtr &o) noexcept
{
    qDebug() << "invoke3" << o << o->m_text << o->m_t << o->m_tt;
}

QJsonObject InvokeTest::invoke4() noexcept
{
    ObjectPtr o = ObjectPtr::create();
    for(int i = 0; i < 10; ++i) {
        TmpPtr t = TmpPtr::create();
        t->m_aaa = QString::number(i);
        o->m_t.append(t);
    }
    o->m_tt.insert(1, o->m_t);
    o->m_text = "cccccccccccccc";
    return McJsonUtils::toJson(o);
}

GadgetTestPtr InvokeTest::invoke5(const GadgetTestPtr &o) noexcept
{
    qDebug() << "invoke5" << o << o->text << o->t << o->tt << o->t2;
    return o;
}

GadgetTest *InvokeTest::invoke6(GadgetTest *o) noexcept
{
    qDebug() << "invoke6" << o << o->text << o->t << o->tt << o->t2;
    return o;
}

void InvokeTest::start() noexcept {
    qDebug() << "InvokeTest start construct";
}

#include <QTimer>
void InvokeTest::end() noexcept {
    qDebug() << "InvokeTest construct finished" << thread() << QThread::currentThread();
    QTimer::singleShot(1000, []() {
        $.invoke("con.invoke4").then([](const QVariant &var) {
            qDebug() << "---------------" << var;
        });
        QJsonObject jsonObj;
        jsonObj.insert("text", "text");
        QJsonObject tObj;
        tObj.insert("aaa", "aaa");
        QJsonArray tArr;
        tArr.append(tObj);
        jsonObj.insert("t", tArr);
        QJsonObject ttObj;
        ttObj.insert("111", tArr);
        jsonObj.insert("tt", ttObj);
        QJsonObject t2Obj;
        t2Obj.insert("text2", "text2");
        jsonObj.insert("t2", t2Obj);
        auto gp = McJsonUtils::fromJson<GadgetTestPtr>(jsonObj);
        auto g = McJsonUtils::fromJson<GadgetTest *>(jsonObj);
        qDebug() << "end-----" << gp << gp->t2->text2;
        qDebug() << "end-----" << g << g->t2->text2;
        QJsonArray jsonArr;
        jsonArr.append(jsonObj);
        auto ga = McJsonUtils::fromJson<QList<GadgetTestPtr>>(jsonArr);
        qDebug() << ga.front()->t2->text2;
    });
}

void InvokeTest::threadEnd() noexcept {
    qDebug() << "InvokeTest construct threadEnd" << thread() << QThread::currentThread();
}
