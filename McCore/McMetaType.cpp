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
#include "McMetaType.h"

MC_GLOBAL_STATIC_BEGIN(coreMetaTypeStaticData)
QVector<McMetaType> metaTypes;
QVector<McListMetaType> listMetaTypes;
QVector<McMapMetaType> mapMetaTypes;
QVector<McPairMetaType> pairMetaTypes;
MC_GLOBAL_STATIC_END(coreMetaTypeStaticData)

////////////////////////////////////////////
void McMetaType::registerMetaType(const McMetaType &type) noexcept
{
    if (!type.isValid() || type.d->isRegistered.loadRelaxed()) {
        return;
    }
#ifdef MC_USE_QT5
    type.d->pMetaType();
    type.d->sMetaType();
#else
    type.d->metaType.id();
    type.d->pMetaType.id();
    type.d->sMetaType.id();
#endif
    type.d->isRegistered.storeRelaxed(true);
    coreMetaTypeStaticData->metaTypes.append(type);
}

#ifdef MC_USE_QT6
McMetaType McMetaType::fromQMetaType(const QMetaType &type) noexcept
{
    auto itr = std::find_if(coreMetaTypeStaticData->metaTypes.constBegin(),
        coreMetaTypeStaticData->metaTypes.constEnd(), [&type](const McMetaType &t) { return type == t.d->metaType; });
    if (itr == coreMetaTypeStaticData->metaTypes.constEnd()) {
        return McMetaType();
    }
    return *itr;
}

McMetaType McMetaType::fromWQMetaType(const QMetaType &type) noexcept
{
    auto itr = std::find_if(coreMetaTypeStaticData->metaTypes.constBegin(),
        coreMetaTypeStaticData->metaTypes.constEnd(), [&type](const McMetaType &t) { return type == t.d->wMetaType; });
    if (itr == coreMetaTypeStaticData->metaTypes.constEnd()) {
        return McMetaType();
    }
    return *itr;
}

McMetaType McMetaType::fromTQMetaType(const QMetaType &type) noexcept
{
    auto itr = std::find_if(coreMetaTypeStaticData->metaTypes.constBegin(),
        coreMetaTypeStaticData->metaTypes.constEnd(), [&type](const McMetaType &t) { return type == t.d->tMetaType; });
    if (itr == coreMetaTypeStaticData->metaTypes.constEnd()) {
        return McMetaType();
    }
    return *itr;
}

McMetaType McMetaType::fromWTypeName(const QByteArray &typeName) noexcept
{
    auto itr = std::find_if(coreMetaTypeStaticData->metaTypes.constBegin(),
        coreMetaTypeStaticData->metaTypes.constEnd(),
        [&typeName](const McMetaType &t) { return typeName == t.d->wMetaType.name(); });
    if (itr == coreMetaTypeStaticData->metaTypes.constEnd()) {
        return McMetaType();
    }
    return *itr;
}

McMetaType McMetaType::fromTTypeName(const QByteArray &typeName) noexcept
{
    auto itr = std::find_if(coreMetaTypeStaticData->metaTypes.constBegin(),
        coreMetaTypeStaticData->metaTypes.constEnd(),
        [&typeName](const McMetaType &t) { return typeName == t.d->tMetaType.name(); });
    if (itr == coreMetaTypeStaticData->metaTypes.constEnd()) {
        return McMetaType();
    }
    return *itr;
}
#endif

#ifdef MC_USE_QT5
McMetaType McMetaType::fromPQMetaType(int type) noexcept
{
    auto itr = std::find_if(coreMetaTypeStaticData->metaTypes.constBegin(),
        coreMetaTypeStaticData->metaTypes.constEnd(),
        [&type](const McMetaType &t) { return type == t.d->pMetaType(); });
#else
McMetaType McMetaType::fromPQMetaType(const QMetaType &type) noexcept
{
    auto itr = std::find_if(coreMetaTypeStaticData->metaTypes.constBegin(),
        coreMetaTypeStaticData->metaTypes.constEnd(), [&type](const McMetaType &t) { return type == t.d->pMetaType; });
#endif
    if (itr == coreMetaTypeStaticData->metaTypes.constEnd()) {
        return McMetaType();
    }
    return *itr;
}

#ifdef MC_USE_QT5
McMetaType McMetaType::fromSQMetaType(int type) noexcept
{
    auto itr = std::find_if(coreMetaTypeStaticData->metaTypes.constBegin(),
        coreMetaTypeStaticData->metaTypes.constEnd(),
        [&type](const McMetaType &t) { return type == t.d->sMetaType(); });
#else
McMetaType McMetaType::fromSQMetaType(const QMetaType &type) noexcept
{
    auto itr = std::find_if(coreMetaTypeStaticData->metaTypes.constBegin(),
        coreMetaTypeStaticData->metaTypes.constEnd(), [&type](const McMetaType &t) { return type == t.d->sMetaType; });
#endif
    if (itr == coreMetaTypeStaticData->metaTypes.constEnd()) {
        return McMetaType();
    }
    return *itr;
}

#ifdef MC_USE_QT5
McMetaType McMetaType::fromFuzzyQMetaType(int type) noexcept
{
    McMetaType metaType;
    auto flags = QMetaType::typeFlags(type);
    if (flags.testFlag(QMetaType::PointerToQObject) || flags.testFlag(QMetaType::PointerToGadget)) {
        metaType = McMetaType::fromPQMetaType(type);
    } else {
        metaType = McMetaType::fromSQMetaType(type);
    }
    if (!metaType.isValid()) {
        metaType = McMetaType::fromPQMetaType(type);
    }
    return metaType;
}
#else
McMetaType McMetaType::fromFuzzyQMetaType(const QMetaType &type) noexcept
{
    McMetaType metaType;
    auto flags = type.flags();
    if (flags.testFlag(QMetaType::IsPointer)) {
        metaType = McMetaType::fromPQMetaType(type);
    } else if (flags.testFlag(QMetaType::WeakPointerToQObject)) {
        metaType = McMetaType::fromWQMetaType(type);
    } else if (flags.testFlag(QMetaType::TrackingPointerToQObject)) {
        metaType = McMetaType::fromTQMetaType(type);
    } else {
        metaType = McMetaType::fromSQMetaType(type);
    }
    return metaType;
}
#endif

McMetaType McMetaType::fromTypeName(const QByteArray &typeName) noexcept
{
    auto itr = std::find_if(coreMetaTypeStaticData->metaTypes.constBegin(),
        coreMetaTypeStaticData->metaTypes.constEnd(), [&typeName](const McMetaType &t) {
#ifdef MC_USE_QT5
            return typeName == t.d->name;
#else
            return typeName == t.d->metaType.name();
#endif
        });
    if (itr == coreMetaTypeStaticData->metaTypes.constEnd()) {
        return McMetaType();
    }
    return *itr;
}

McMetaType McMetaType::fromPTypeName(const QByteArray &typeName) noexcept
{
    auto itr = std::find_if(coreMetaTypeStaticData->metaTypes.constBegin(),
        coreMetaTypeStaticData->metaTypes.constEnd(), [&typeName](const McMetaType &t) {
#ifdef MC_USE_QT5
            return typeName == QMetaType::typeName(t.d->pMetaType());
#else
            return typeName == t.d->pMetaType.name();
#endif
        });
    if (itr == coreMetaTypeStaticData->metaTypes.constEnd()) {
        return McMetaType();
    }
    return *itr;
}

McMetaType McMetaType::fromSTypeName(const QByteArray &typeName) noexcept
{
    auto itr = std::find_if(coreMetaTypeStaticData->metaTypes.constBegin(),
        coreMetaTypeStaticData->metaTypes.constEnd(), [&typeName](const McMetaType &t) {
#ifdef MC_USE_QT5
            return typeName == QMetaType::typeName(t.d->sMetaType());
#else
            return typeName == t.d->sMetaType.name();
#endif
        });
    if (itr == coreMetaTypeStaticData->metaTypes.constEnd()) {
        return McMetaType();
    }
    return *itr;
}

QVector<McMetaType> McMetaType::metaTypes() noexcept
{
    return coreMetaTypeStaticData->metaTypes;
}

void *McMetaType::createPointer() const noexcept
{
    if (!isValid()) {
        return nullptr;
    }
#ifdef MC_USE_QT5
    if (d->createPointer == nullptr) {
        return nullptr;
    }
    return d->createPointer();
#else
    return d->metaType.create();
#endif
}

QVariant McMetaType::createQVariantPointer() const noexcept
{
    void *star = createPointer();
    if (Q_UNLIKELY(star == nullptr)) {
        return QVariant();
    }
    return QVariant(pMetaType(), &star);
}

QVariant McMetaType::createSharedPointer(void *copy) const noexcept
{
    if (!isValid() || d->createSharedPointer == nullptr) {
        return QVariant();
    }
    return d->createSharedPointer(copy);
}

void McMetaType::addParentMetaType(const McMetaType &type) const noexcept
{
    if (!isValid() || !type.isValid()) {
        return;
    }
    if (d->parents == nullptr) {
        //! 由于d为编译期静态变量，全局唯一且整个程序生命周期均存在，此处可不析构
        d->parents = new QSet<McMetaType>();
    }
    d->parents->insert(type);
}

QSet<McMetaType> McMetaType::parentMetaTypes() const noexcept
{
    if (!isValid() || d->parents == nullptr) {
        return QSet<McMetaType>();
    }
    return *d->parents;
}

////////////////////////////////////////////
void McListMetaType::registerMetaType(const McListMetaType &type) noexcept
{
    if (!type.isValid() || type.d->isRegistered.loadRelaxed()) {
        return;
    }
    type.d->isRegistered.storeRelaxed(true);
#ifdef MC_USE_QT5
    type.d->metaType();
    type.d->valueMetaType();
#else
    type.d->metaType.id();
#endif
    coreMetaTypeStaticData->listMetaTypes.append(type);
}

#ifdef MC_USE_QT5
McListMetaType McListMetaType::fromQMetaType(int type) noexcept
{
    auto itr = std::find_if(coreMetaTypeStaticData->listMetaTypes.constBegin(),
        coreMetaTypeStaticData->listMetaTypes.constEnd(),
        [&type](const McListMetaType &t) { return type == t.d->metaType(); });
#else
McListMetaType McListMetaType::fromQMetaType(const QMetaType &type) noexcept
{
    auto itr = std::find_if(coreMetaTypeStaticData->listMetaTypes.constBegin(),
        coreMetaTypeStaticData->listMetaTypes.constEnd(),
        [&type](const McListMetaType &t) { return type == t.d->metaType; });
#endif
    if (itr == coreMetaTypeStaticData->listMetaTypes.constEnd()) {
        return McListMetaType();
    }
    return *itr;
}

QVector<McListMetaType> McListMetaType::metaTypes() noexcept
{
    return coreMetaTypeStaticData->listMetaTypes;
}

////////////////////////////////////////////
void McMapMetaType::registerMetaType(const McMapMetaType &type) noexcept
{
    if (!type.isValid() || type.d->isRegistered.loadRelaxed()) {
        return;
    }
    type.d->isRegistered.storeRelaxed(true);
#ifdef MC_USE_QT5
    type.d->metaType();
    type.d->keyMetaType();
    type.d->valueMetaType();
#else
    type.d->metaType.id();
#endif
    coreMetaTypeStaticData->mapMetaTypes.append(type);
}

#ifdef MC_USE_QT5
McMapMetaType McMapMetaType::fromQMetaType(int type) noexcept
{
    auto itr = std::find_if(coreMetaTypeStaticData->mapMetaTypes.constBegin(),
        coreMetaTypeStaticData->mapMetaTypes.constEnd(),
        [&type](const McMapMetaType &t) { return type == t.d->metaType(); });
#else
McMapMetaType McMapMetaType::fromQMetaType(const QMetaType &type) noexcept
{
    auto itr = std::find_if(coreMetaTypeStaticData->mapMetaTypes.constBegin(),
        coreMetaTypeStaticData->mapMetaTypes.constEnd(),
        [&type](const McMapMetaType &t) { return type == t.d->metaType; });
#endif
    if (itr == coreMetaTypeStaticData->mapMetaTypes.constEnd()) {
        return McMapMetaType();
    }
    return *itr;
}

QVector<McMapMetaType> McMapMetaType::metaTypes() noexcept
{
    return coreMetaTypeStaticData->mapMetaTypes;
}

////////////////////////////////////////////
void McPairMetaType::registerMetaType(const McPairMetaType &type) noexcept
{
    if (!type.isValid() || type.d->isRegistered.loadRelaxed()) {
        return;
    }
    type.d->isRegistered.storeRelaxed(true);
#ifdef MC_USE_QT5
    type.d->metaType();
    type.d->keyMetaType();
    type.d->valueMetaType();
#else
    type.d->metaType.id();
#endif
    coreMetaTypeStaticData->pairMetaTypes.append(type);
}

#ifdef MC_USE_QT5
McPairMetaType McPairMetaType::fromQMetaType(int type) noexcept
{
    auto itr = std::find_if(coreMetaTypeStaticData->pairMetaTypes.constBegin(),
        coreMetaTypeStaticData->pairMetaTypes.constEnd(),
        [&type](const McPairMetaType &t) { return type == t.d->metaType(); });
#else
McPairMetaType McPairMetaType::fromQMetaType(const QMetaType &type) noexcept
{
    auto itr = std::find_if(coreMetaTypeStaticData->pairMetaTypes.constBegin(),
        coreMetaTypeStaticData->pairMetaTypes.constEnd(),
        [&type](const McPairMetaType &t) { return type == t.d->metaType; });
#endif
    if (itr == coreMetaTypeStaticData->pairMetaTypes.constEnd()) {
        return McPairMetaType();
    }
    return *itr;
}

QVector<McPairMetaType> McPairMetaType::metaTypes() noexcept
{
    return coreMetaTypeStaticData->pairMetaTypes;
}
