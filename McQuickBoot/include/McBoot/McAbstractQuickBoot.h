/*
 * MIT License
 *
 * Copyright (c) 2021 mrcao20
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once

#include "IMcQuickBoot.h"

#include "McBoot/Controller/impl/McCppResponse.h"
#include "McBoot/Requestor/McCppRequestor.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractQuickBoot);

class MCQUICKBOOT_EXPORT McAbstractQuickBoot : public QObject, public IMcQuickBoot
{
    Q_OBJECT
    MC_DECL_INIT(McAbstractQuickBoot)
public:
    using QObject::connect;

    explicit McAbstractQuickBoot(QObject *parent = nullptr);
    ~McAbstractQuickBoot() override;

    static QMetaObject::Connection connect(const QString &sender,
                                           const QString &signal,
                                           const QString &receiver,
                                           const QString &slot,
                                           Qt::ConnectionType type = Qt::AutoConnection) noexcept;
    static QMetaObject::Connection connect(const QString &sender,
                                           const QString &signal,
                                           QObject *receiver,
                                           const QString &slot,
                                           Qt::ConnectionType type = Qt::AutoConnection) noexcept;
    static bool disconnect(const QString &sender,
                           const QString &signal,
                           const QString &receiver,
                           const QString &slot) noexcept;

    static bool disconnect(const QString &sender,
                           const QString &signal,
                           QObject *receiver,
                           const QString &slot) noexcept;
    static QSharedPointer<McAbstractQuickBoot> instance() noexcept;
    static void addConfigPath(const QString &path) noexcept;

    McCppRequestor &requestor() const noexcept;

protected:
    static void setInstance(const QSharedPointer<McAbstractQuickBoot> &ins) noexcept;

    virtual void initContainer() const noexcept = 0;

    void doRefresh(const QStringList &preloadBeans = QStringList()) noexcept;

private:
    MC_DECL_PRIVATE(McAbstractQuickBoot)
};

MC_DECL_POINTER(McAbstractQuickBoot);
