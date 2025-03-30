/*
 * Copyright (c) 2021 mrcao20/mrcao20@163.com
 * McQuickBoot is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#pragma once

#include "../McQuickBootGlobal.h"

MC_FORWARD_DECL_PRIVATE_DATA(McResult)

class MC_QUICKBOOT_EXPORT McResult : public QObject
{
    Q_OBJECT
    MC_DECL_INIT(McResult)
    MC_JSON_SERIALIZATION()
    Q_PROPERTY(bool success READ isSuccess)
    Q_PROPERTY(QString errMsg READ errMsg)
    Q_PROPERTY(QVariant result READ result)
public:
    explicit McResult(QObject *parent = nullptr) noexcept;
    ~McResult() override;

    bool isSuccess() const noexcept;
    void setSuccess(bool val) noexcept;

    QString errMsg() const noexcept;
    void setErrMsg(const QString &val) noexcept;

    QVariant result() const noexcept;
    void setResult(const QVariant &val) noexcept;

    //! 内部参数
    bool isInternalError() const noexcept;
    void setInternalError(bool val) noexcept;

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
    friend MC_QUICKBOOT_EXPORT QDebug operator<<(QDebug dbg, const McResult &r);
    friend MC_QUICKBOOT_EXPORT QDebug operator<<(QDebug dbg, McResult *r);
    friend MC_QUICKBOOT_EXPORT QDebug operator<<(QDebug dbg, const QSharedPointer<McResult> &r);

private:
    MC_DECL_PRIVATE(McResult)
};

MC_DECL_METATYPE(McResult)
