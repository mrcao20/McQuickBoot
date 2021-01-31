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
    static QSharedPointer<McAbstractQuickBoot> instance() noexcept;

    McCppRequestor &requestor() const noexcept;

protected:
    static void setInstance(const QSharedPointer<McAbstractQuickBoot> &ins) noexcept;

    virtual void initContainer() const noexcept = 0;

    void setRequestor(const McCppRequestorPtr &req) noexcept;
    void doRefresh() noexcept;

private:
    MC_DECL_PRIVATE(McAbstractQuickBoot)
};

MC_DECL_POINTER(McAbstractQuickBoot);
