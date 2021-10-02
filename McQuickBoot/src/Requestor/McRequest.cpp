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

McPause McRequest::pause() const noexcept
{
    return d->pause;
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

void McRequest::setPause(const McPause &val) noexcept
{
    d->pause = val;
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
