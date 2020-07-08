#include "QmlSocketTest.h"

#include <QtDebug>
#include <QJsonObject>

MC_INIT(QmlSocketTest)
MC_REGISTER_BEAN_FACTORY(QmlSocketTest)
MC_INIT_END

QmlSocketTest::QmlSocketTest(QObject *parent)
    : QObject(parent)
{
}

QmlSocketTest::~QmlSocketTest(){}

void QmlSocketTest::onOpen(const IMcSessionPtr &session) noexcept {
    qDebug() << "onOpen" << session;
}

void QmlSocketTest::onClose(const IMcSessionPtr &session) noexcept {
    qDebug() << "onClose" << session;
}

void QmlSocketTest::onError(const IMcSessionPtr &session) noexcept {
    qDebug() << "onError" << session;
}

void QmlSocketTest::onMessage(const IMcSessionPtr &session, const QString &msg) noexcept {
    qDebug() << "onMessage" << session << msg;
    auto o = QJsonObject({{"a", "b"}});
    session->send(o);
}
