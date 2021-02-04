#pragma once

#include "McAbstractRequestor.h"

#include <QJSValue>

MC_FORWARD_DECL_PRIVATE_DATA(McQmlRequestor);

class McQmlResponse;

class MCQUICKBOOT_EXPORT McQmlRequestor : public McAbstractRequestor
{
    Q_OBJECT
    MC_DECL_INIT(McQmlRequestor)
    MC_TYPELIST(McAbstractRequestor)
    MC_COMPONENT
    MC_BEANNAME("qmlRequestor")
    MC_POINTER(true)
public:
    using McAbstractRequestor::connect;
    using McAbstractRequestor::disconnect;

    Q_INVOKABLE explicit McQmlRequestor(QObject *parent = nullptr);
    ~McQmlRequestor() override;

    Q_INVOKABLE McQmlResponse *invoke(const QString &uri,
                                      const QJSValue &data1 = QJSValue(),
                                      const QJSValue &data2 = QJSValue()) noexcept;
    Q_INVOKABLE QJSValue syncInvoke(const QString &uri) noexcept;
    Q_INVOKABLE QJSValue syncInvoke(const QString &uri, const QJSValue &data) noexcept;
    Q_INVOKABLE QString connect(const QString &beanName,
                                const QString &signal,
                                const QJSValue &callback) noexcept;
    Q_INVOKABLE void disconnect(const QString &uuid) noexcept;
    Q_INVOKABLE void disconnect(const QString &beanName,
                                const QString &signal,
                                const QJSValue &callback = QJSValue()) noexcept;

private:
    MC_DECL_PRIVATE(McQmlRequestor)
};

MC_DECL_METATYPE(McQmlRequestor)
