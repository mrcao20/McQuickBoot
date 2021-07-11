#pragma once

#include "../../McBootGlobal.h"

MC_FORWARD_DECL_PRIVATE_DATA(McResult);

class MCQUICKBOOT_EXPORT McResult : public QObject 
{
    Q_OBJECT
    MC_DECL_INIT(McResult)
    MC_JSON_SERIALIZATION()
    Q_PROPERTY(bool success READ isSuccess)
    Q_PROPERTY(QString errMsg READ errMsg)
    Q_PROPERTY(QVariant result READ result)
public:
    explicit McResult(QObject *parent = nullptr);
    ~McResult() override;
    
    bool isSuccess() const noexcept;
    void setSuccess(bool val) noexcept;
    
    QString errMsg() const noexcept;
    void setErrMsg(const QString &val) noexcept;
    
    QVariant result() const noexcept;
    void setResult(const QVariant &val) noexcept;

    static QSharedPointer<McResult> ok(const QVariant &val) noexcept
    {
        auto result = QSharedPointer<McResult>::create();
        result->setSuccess(true);
        result->setResult(val);
        return result;
    }

    static QSharedPointer<McResult> fail(const QString &val) noexcept
    {
        auto result = QSharedPointer<McResult>::create();
        result->setSuccess(false);
        result->setErrMsg(val);
        return result;
    }

    friend MCQUICKBOOT_EXPORT QDebug operator<<(QDebug dbg, McResult *r);
    friend MCQUICKBOOT_EXPORT QDebug operator<<(QDebug dbg, const QSharedPointer<McResult> &r);

private:
    bool isInternalError() const noexcept;
    void setInternalError(bool val) noexcept;

private:
    MC_DECL_PRIVATE(McResult)

    friend class McControllerContainer;
    friend class McAbstractResponse;
};

MC_DECL_METATYPE(McResult)
