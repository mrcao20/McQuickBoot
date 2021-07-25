#include "McBoot/Requestor/McRequest.h"

MC_INIT(McRequest)
qRegisterMetaType<McRequest>();
MC_INIT_END

McRequest::McRequest() noexcept
{
    d = new McRequestSharedData();
}

McRequest::~McRequest() {}

McCancel McRequest::cancel() const noexcept
{
    return d->cancel;
}

McProgress McRequest::progress() const noexcept
{
    return d->progress;
}

int McRequest::count() const noexcept
{
    return d->params.size();
}

QVariant McRequest::variant(int i) const noexcept
{
    if (i < 0 || i >= d->params.size()) {
        return QVariant();
    }
    return d->params.at(i);
}

void McRequest::setCancel(const McCancel &val) noexcept
{
    d->cancel = val;
}

void McRequest::setProgress(const McProgress &val) noexcept
{
    d->progress = val;
}

void McRequest::setParams(const QVariantList &val) noexcept
{
    d->params = val;
}
