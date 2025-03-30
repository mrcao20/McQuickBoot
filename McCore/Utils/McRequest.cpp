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
#include "McRequest.h"

MC_DECL_SHARED_PRIVATE_DATA2(McRequest)
{
    McCancel cancel;
    McPause pause;
    McProgress progress;
    QVariantList params;
    qsizetype rollIndex{0};
};

McRequest::McRequest() noexcept
{
    MC_NEW_SHARED_PRIVATE_DATA(McRequest);
}

McRequest::~McRequest() {}

McRequest::McRequest(const McRequest &o) noexcept
    : d(o.d)
{
}

McRequest &McRequest::operator=(const McRequest &o) noexcept
{
    McRequest copy(o);
    d.swap(copy.d);
    return *this;
}

McRequest::McRequest(McRequest &&o) noexcept
    : d(o.d)
{
    o.d.reset();
}

McRequest &McRequest::operator=(McRequest &&o) noexcept
{
    McRequest copy(std::move(o));
    d.swap(copy.d);
    return *this;
}

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

QVariantList McRequest::params() const noexcept
{
    return d->params;
}

qsizetype McRequest::count() const noexcept
{
    return d->params.size();
}

QVariant McRequest::at(qsizetype i) const noexcept
{
    if (!Mc::isContains(i, d->params)) {
        return QVariant();
    }
    return d->params.at(i);
}

QVariant McRequest::rollAt() const noexcept
{
    QVariant var = at(d->rollIndex);
    ++d->rollIndex;
    if (d->rollIndex >= d->params.size())
        d->rollIndex = 0;
    return var;
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

void McRequest::addParam(const QVariant &var) noexcept
{
    d->params.append(var);
}

///////////////////////////////////////////////
#ifdef MC_USE_QT6
Q_DECL_CONST_FUNCTION size_t qHash(QMetaType key, size_t seed) noexcept
{
    return qHash(key.id(), seed);
}
#endif
MC_GLOBAL_STATIC_BEGIN(staticData)
#ifdef MC_USE_QT5
QHash<int, QList<int>> customRequestMetaTypes;
QHash<int, McPrivate::ICustomRequestBuilderPtr> customRequestBuilders;
#else
QHash<QMetaType, QList<QMetaType>> customRequestMetaTypes;
QHash<QMetaType, McPrivate::ICustomRequestBuilderPtr> customRequestBuilders;
#endif
MC_GLOBAL_STATIC_END(staticData)

namespace McPrivate {
#ifdef MC_USE_QT5
void registerCustomRequest(int mainMetaType, const QList<int> &childMetaTypes) noexcept
{
    if (staticData->customRequestMetaTypes.contains(mainMetaType)) {
        return;
    }
    staticData->customRequestMetaTypes.insert(mainMetaType, childMetaTypes);
}

void registerCustomRequestBuilder(int mainMetaType, const ICustomRequestBuilderPtr &builder) noexcept
{
    if (staticData->customRequestBuilders.contains(mainMetaType)) {
        return;
    }
    staticData->customRequestBuilders.insert(mainMetaType, builder);
}

QVariant buildCustomRequest(int mainMetaType, const QVariantList &vals, const McRequest &request) noexcept
{
    if (!staticData->customRequestBuilders.contains(mainMetaType)) {
        return QVariant();
    }
    return staticData->customRequestBuilders.value(mainMetaType)->build(vals, request);
}

QHash<int, QList<int>> getAllCustomRequestMetaTypes() noexcept
{
    return staticData->customRequestMetaTypes;
}

QList<int> getCustomRequestMetaType(int mainMetaType) noexcept
{
    return staticData->customRequestMetaTypes.value(mainMetaType);
}

bool isContainedCustomRequest(int mainMetaType) noexcept
{
    return staticData->customRequestMetaTypes.contains(mainMetaType);
}
#else
void registerCustomRequest(QMetaType mainMetaType, const QList<QMetaType> &childMetaTypes) noexcept
{
    if (staticData->customRequestMetaTypes.contains(mainMetaType)) {
        return;
    }
    staticData->customRequestMetaTypes.insert(mainMetaType, childMetaTypes);
}

void registerCustomRequestBuilder(QMetaType mainMetaType, const ICustomRequestBuilderPtr &builder) noexcept
{
    if (staticData->customRequestBuilders.contains(mainMetaType)) {
        return;
    }
    staticData->customRequestBuilders.insert(mainMetaType, builder);
}

QVariant buildCustomRequest(QMetaType mainMetaType, const QVariantList &vals, const McRequest &request) noexcept
{
    if (!staticData->customRequestBuilders.contains(mainMetaType)) {
        return QVariant();
    }
    return staticData->customRequestBuilders.value(mainMetaType)->build(vals, request);
}

QHash<QMetaType, QList<QMetaType>> getAllCustomRequestMetaTypes() noexcept
{
    return staticData->customRequestMetaTypes;
}

QList<QMetaType> getCustomRequestMetaType(QMetaType mainMetaType) noexcept
{
    return staticData->customRequestMetaTypes.value(mainMetaType);
}

bool isContainedCustomRequest(QMetaType mainMetaType) noexcept
{
    return staticData->customRequestMetaTypes.contains(mainMetaType);
}
#endif
} // namespace McPrivate
