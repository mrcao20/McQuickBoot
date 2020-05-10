#include "InvokeTest.h"

#include <QThread>
#include <QDebug>

#include <McIoc/ApplicationContext/McContainerGlobal.h>
#include <McBoot/Controller/impl/McResult.h>

MC_INIT(InvokeTest)
MC_REGISTER_COMPONENT(InvokeTest)
MC_INIT_END

InvokeTest::InvokeTest(QObject *parent) 
    : ClassTestB(parent)
{
}

void InvokeTest::invoke1() noexcept {
    qDebug() << "invoke1";
}

McResult *InvokeTest::invoke2() noexcept {
    static quint64 i = 0;
    qDebug() << "-----------" << i++;
    auto obj = McResult::fail("");
    return obj;
}

void InvokeTest::invoke3(const ObjectPtr &o) noexcept {
    qDebug() << "-------" << o << o->m_text << o->m_t << o->m_t->m_aaa;
}

void InvokeTest::start() noexcept {
    qDebug() << "InvokeTest start construct";
}

void InvokeTest::end() noexcept {
    qDebug() << "InvokeTest construct finished" << thread() << QThread::currentThread();
}

void InvokeTest::threadEnd() noexcept {
    qDebug() << "InvokeTest construct threadEnd" << thread() << QThread::currentThread();
}
