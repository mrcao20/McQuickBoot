#pragma once

#include "../../Utils/McCancel.h"
#include "../../Utils/McProgress.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractResponse);

/*!
 * \brief The McAbstractResponse class
 * 此抽象类的子类在每次请求时都会构造一个新的对象，
 * 可以设计成使用对象池来优化
 */
class MCQUICKBOOT_EXPORT McAbstractResponse : public QObject
{
    Q_OBJECT
    MC_DECL_INIT(McAbstractResponse)
public:
    explicit McAbstractResponse(QObject *parent = nullptr);
    ~McAbstractResponse() override;

    Q_INVOKABLE void cancel() noexcept;
    Q_INVOKABLE bool isCanceled() const noexcept;

    QVariant body() const noexcept;
    void setBody(const QVariant &var) noexcept;

    bool isAsyncCall() const noexcept;
    void setAsyncCall(bool val) noexcept;

    Q_INVOKABLE void attach(QObject *obj) noexcept;
    Q_INVOKABLE void detach() noexcept;

protected:
    void customEvent(QEvent *event) override;

    virtual void callCallback() noexcept = 0;
    virtual void callError() noexcept = 0;

    McProgress &getProgress() const noexcept;
    McCancel &getCancel() const noexcept;

private:
    void call() noexcept;

private:
    MC_DECL_PRIVATE(McAbstractResponse)

    friend class McRequestRunner;
};

Q_DECLARE_METATYPE(McAbstractResponse *)
