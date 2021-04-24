#pragma once

#include "McAbstractResponse.h"

QT_BEGIN_NAMESPACE
class QJSValue;
class QJSEngine;
QT_END_NAMESPACE

MC_FORWARD_DECL_PRIVATE_DATA(McQmlResponse);

class MCQUICKBOOT_EXPORT McQmlResponse : public McAbstractResponse
{
    Q_OBJECT
    MC_DECL_INIT(McQmlResponse)
public:
    explicit McQmlResponse(QObject *parent = nullptr);
    ~McQmlResponse() override;
    
    Q_INVOKABLE McQmlResponse *then(const QJSValue &callback) noexcept;
    Q_INVOKABLE McQmlResponse *syncThen(const QJSValue &callback) noexcept;
    Q_INVOKABLE McQmlResponse *asyncThen(const QJSValue &callback) noexcept;
    Q_INVOKABLE McQmlResponse *error(const QJSValue &func) noexcept;

protected:
    void callCallback() noexcept override;
    void callError() noexcept override;

private:
    void call(QJSValue &func) noexcept;

private:
    MC_DECL_PRIVATE(McQmlResponse)
};

Q_DECLARE_METATYPE(McQmlResponse *)
