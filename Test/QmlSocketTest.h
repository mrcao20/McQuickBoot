#pragma once

#include <QObject>

#include <McIoc/McMacroGlobal.h>
#include <McIoc/ApplicationContext/McContainerGlobal.h>
#include <McBoot/Socket/IMcSession.h>

class QmlSocketTest : public QObject {
    Q_OBJECT
    Q_CLASSINFO(MC_COMPONENT, MC_QML_SOCKET)
    Q_CLASSINFO(MC_BEANNAME, "socket")
    MC_DECL_INIT(QmlSocketTest)
public:
    Q_INVOKABLE explicit QmlSocketTest(QObject *parent = nullptr);
    ~QmlSocketTest();
    
    Q_INVOKABLE
    MC_ON_OPEN
    void onOpen(const IMcSessionPtr &session) noexcept;
    
    Q_INVOKABLE
    MC_ON_CLOSE
    void onClose(const IMcSessionPtr &session) noexcept;
    
    Q_INVOKABLE
    MC_ON_ERROR
    void onError(const IMcSessionPtr &session) noexcept;
    
    Q_INVOKABLE
    MC_ON_MESSAGE
    void onMessage(const IMcSessionPtr &session, const QString &msg) noexcept;
    
};

MC_DECL_METATYPE(QmlSocketTest)
