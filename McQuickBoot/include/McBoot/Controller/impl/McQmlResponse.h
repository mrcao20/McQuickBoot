#pragma once

#include "../../McBootGlobal.h"

QT_BEGIN_NAMESPACE
class QJSValue;
class QJSEngine;
QT_END_NAMESPACE

MC_FORWARD_DECL_PRIVATE_DATA(McQmlResponse);

class MCQUICKBOOT_EXPORT McQmlResponse : public QObject 
{
    Q_OBJECT
    MC_DECL_INIT(McQmlResponse)
public:
    explicit McQmlResponse(QObject *parent = nullptr);
    ~McQmlResponse() override;
    
    void setBody(const QVariant &var) noexcept;

    Q_INVOKABLE McQmlResponse *then(const QJSValue &callback) noexcept;
    Q_INVOKABLE McQmlResponse *syncThen(const QJSValue &callback) noexcept;
    
    McQmlResponse *then(const std::function<void(const QVariant &)> &callback) noexcept;
    McQmlResponse *syncThen(const std::function<void(const QVariant &)> &callback) noexcept;
    
protected:
    void customEvent(QEvent *event) override;
    
private:
    void callCallback() noexcept;
    
private:
    MC_DECL_PRIVATE(McQmlResponse)
};

Q_DECLARE_METATYPE(McQmlResponse *)
