#pragma once

#include "../../McBootGlobal.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractResponse);

class MCQUICKBOOT_EXPORT McAbstractResponse : public QObject
{
    Q_OBJECT
    MC_DECL_INIT(McAbstractResponse)
public:
    explicit McAbstractResponse(QObject *parent = nullptr);
    ~McAbstractResponse() override;

    QVariant body() const noexcept;
    void setBody(const QVariant &var) noexcept;

    bool isAsyncCall() const noexcept;
    void setAsyncCall(bool val) noexcept;

protected:
    void customEvent(QEvent *event) override;

    virtual void callCallback() noexcept = 0;

private:
    MC_DECL_PRIVATE(McAbstractResponse)
};

Q_DECLARE_METATYPE(McAbstractResponse *)
