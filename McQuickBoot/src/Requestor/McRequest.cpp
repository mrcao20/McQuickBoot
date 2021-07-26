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

MC_GLOBAL_STATIC_BEGIN(requestStaticData)
QHash<int, QList<int>> customRequestIds;
QHash<int, Mc::QuickBoot::Private::ICustomRequestBuilderPtr> customRequestBuilders;
MC_GLOBAL_STATIC_END(requestStaticData)

namespace Mc::QuickBoot::Private {

void registerCustomRequest(int mainId, const QList<int> &childrenId) noexcept
{
    if (requestStaticData->customRequestIds.contains(mainId)) {
        return;
    }
    requestStaticData->customRequestIds.insert(mainId, childrenId);
}

void registerCustomRequestBuilder(int mainId, const ICustomRequestBuilderPtr &builder) noexcept
{
    if (requestStaticData->customRequestBuilders.contains(mainId)) {
        return;
    }
    requestStaticData->customRequestBuilders.insert(mainId, builder);
}

QVariant buildCustomRequest(int mainId, const QVariantList &vals, const McRequest &request) noexcept
{
    if (!requestStaticData->customRequestBuilders.contains(mainId)) {
        return QVariant();
    }
    return requestStaticData->customRequestBuilders.value(mainId)->build(vals, request);
}

QHash<int, QList<int>> getAllCustomRequestIds() noexcept
{
    return requestStaticData->customRequestIds;
}

QList<int> getCustomRequestId(int mainId) noexcept
{
    return requestStaticData->customRequestIds.value(mainId);
}

bool isContainedCustomRequest(int mainId) noexcept
{
    return requestStaticData->customRequestIds.contains(mainId);
}

} // namespace Mc::QuickBoot::Private
