#include "InvokeTest.h"

#include <QDebug>

#include <ApplicationContext/McContainerGlobal.h>
#include <Controller/impl/McResult.h>

MC_INIT(InvokeTest)
MC_REGISTER_COMPONENT(InvokeTest)
MC_INIT_END

InvokeTest::InvokeTest(QObject *parent) 
    : QObject(parent)
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
