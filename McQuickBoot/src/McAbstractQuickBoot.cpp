#include "McBoot/McAbstractQuickBoot.h"

namespace {

struct McAbstractQuickBootStaticData
{
    McAbstractQuickBootPtr boot;
};

} // namespace

MC_GLOBAL_STATIC(McAbstractQuickBootStaticData, mcAbstractQuickBootStaticData)

MC_DECL_PRIVATE_DATA(McAbstractQuickBoot)
McCppRequestorPtr requestor;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McAbstractQuickBoot)
MC_DESTROY()
if (!mcAbstractQuickBootStaticData.exists()) {
    return;
}
auto requestor = mcAbstractQuickBootStaticData -> boot -> d -> requestor.data();
mcAbstractQuickBootStaticData->boot->d->requestor.clear();
delete requestor;
mcAbstractQuickBootStaticData->boot.reset();
MC_INIT_END

McAbstractQuickBoot::McAbstractQuickBoot(QObject *parent) : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractQuickBoot);
}

McAbstractQuickBoot::~McAbstractQuickBoot() {}

QMetaObject::Connection McAbstractQuickBoot::connect(const QString &sender,
                                                     const QString &signal,
                                                     const QString &receiver,
                                                     const QString &slot,
                                                     Qt::ConnectionType type) noexcept
{
    return Mc::Ioc::connect(McAbstractQuickBoot::instance()->getApplicationContext(),
                            sender,
                            signal,
                            receiver,
                            slot,
                            type);
}

QMetaObject::Connection McAbstractQuickBoot::connect(const QString &sender,
                                                     const QString &signal,
                                                     QObject *receiver,
                                                     const QString &slot,
                                                     Qt::ConnectionType type) noexcept
{
    return Mc::Ioc::connect(McAbstractQuickBoot::instance()->getApplicationContext(),
                            sender,
                            signal,
                            receiver,
                            slot,
                            type);
}

QSharedPointer<McAbstractQuickBoot> McAbstractQuickBoot::instance() noexcept
{
    McAbstractQuickBootPtr &boot = mcAbstractQuickBootStaticData->boot;
    Q_ASSERT_X(!boot.isNull(), "McAbstractQuickBoot::instance()", "please call init before");
    return boot;
}

McCppRequestor &McAbstractQuickBoot::requestor() const noexcept
{
    return *d->requestor.data();
}

void McAbstractQuickBoot::setInstance(const QSharedPointer<McAbstractQuickBoot> &ins) noexcept
{
    mcAbstractQuickBootStaticData->boot = ins;
}

void McAbstractQuickBoot::setRequestor(const McCppRequestorPtr &req) noexcept
{
    d->requestor = req;
}
