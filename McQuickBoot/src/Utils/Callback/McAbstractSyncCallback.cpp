#include "McBoot/Utils/Callback/Impl/McAbstractSyncCallback.h"

namespace Mc {

struct CallbackEvent : public QEvent
{
    CallbackEvent(const QVariantList &val)
        : QEvent(static_cast<QEvent::Type>(QEvent::Type::User + 1)), varList(val)
    {}
    QVariantList varList;
};

} // namespace Mc

void McAbstractSyncCallback::destroy() noexcept
{
    deleteLater();
}

void McAbstractSyncCallback::call(const QVariantList &varList) noexcept
{
    qApp->postEvent(this, new Mc::CallbackEvent(varList));
}

void McAbstractSyncCallback::customEvent(QEvent *event)
{
    if (event->type() == QEvent::Type::User + 1) {
        Mc::CallbackEvent *e = static_cast<Mc::CallbackEvent *>(event);
        syncCall(e->varList);
    }
}
