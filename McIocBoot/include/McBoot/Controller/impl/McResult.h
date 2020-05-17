#pragma once

#include "../../McBootGlobal.h"

MC_FORWARD_DECL_PRIVATE_DATA(McResult);

class MCIOCBOOT_EXPORT McResult : public QObject {
    Q_OBJECT
    MC_DECL_INIT(McResult)
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
    
    static McResult *ok(const QVariant &val) noexcept {
        McResult *result = new McResult();
        result->setSuccess(true);
        result->setResult(val);
        return result;
    }
    
    static McResult *fail(const QString &val) noexcept {
        McResult *result = new McResult();
        result->setSuccess(false);
        result->setErrMsg(val);
        return result;
    }

private:
    MC_DECL_PRIVATE(McResult)
};

Q_DECLARE_METATYPE(McResult *)
