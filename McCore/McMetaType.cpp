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
#include "McMetaType.h"

MC_GLOBAL_STATIC_BEGIN(coreMetaTypeStaticData)
QVector<McMetaType> metaTypes;
QVector<McListMetaType> listMetaTypes;
QVector<McMapMetaType> mapMetaTypes;
MC_GLOBAL_STATIC_END(coreMetaTypeStaticData)

void McMetaType::registerMetaType(const McMetaType &type) noexcept
{
    if (!type.isValid() || type.d->isRegistered.loadRelaxed()) {
        return;
    }
#ifdef MC_USE_QT6
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

McMetaType McMetaType::fromTypeName(const QByteArray &typeName) noexcept
{
    auto itr = std::find_if(coreMetaTypeStaticData->metaTypes.constBegin(),
        coreMetaTypeStaticData->metaTypes.constEnd(),
        [&typeName](const McMetaType &t) { return typeName == t.d->metaType.name(); });
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
#else
McMetaType McMetaType::fromPQMetaType(const QMetaType &type) noexcept
#endif
{
    auto itr = std::find_if(coreMetaTypeStaticData->metaTypes.constBegin(),
        coreMetaTypeStaticData->metaTypes.constEnd(), [&type](const McMetaType &t) { return type == t.d->pMetaType; });
    if (itr == coreMetaTypeStaticData->metaTypes.constEnd()) {
        return McMetaType();
    }
    return *itr;
}

#ifdef MC_USE_QT5
McMetaType McMetaType::fromSQMetaType(int type) noexcept
#else
McMetaType McMetaType::fromSQMetaType(const QMetaType &type) noexcept
#endif
{
    auto itr = std::find_if(coreMetaTypeStaticData->metaTypes.constBegin(),
        coreMetaTypeStaticData->metaTypes.constEnd(), [&type](const McMetaType &t) { return type == t.d->sMetaType; });
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

McMetaType McMetaType::fromPTypeName(const QByteArray &typeName) noexcept
{
    auto itr = std::find_if(coreMetaTypeStaticData->metaTypes.constBegin(),
        coreMetaTypeStaticData->metaTypes.constEnd(), [&typeName](const McMetaType &t) {
#ifdef MC_USE_QT5
            return typeName == QMetaType::typeName(t.d->pMetaType);
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
            return typeName == QMetaType::typeName(t.d->sMetaType);
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

void McListMetaType::registerMetaType(const McListMetaType &type) noexcept
{
    if (!type.isValid() || type.d->isRegistered.loadRelaxed()) {
        return;
    }
    type.d->isRegistered.storeRelaxed(true);
#ifdef MC_USE_QT6
    type.d->metaType.id();
#endif
    coreMetaTypeStaticData->listMetaTypes.append(type);
}

#ifdef MC_USE_QT5
McListMetaType McListMetaType::fromQMetaType(int type) noexcept
#else
McListMetaType McListMetaType::fromQMetaType(const QMetaType &type) noexcept
#endif
{
    auto itr = std::find_if(coreMetaTypeStaticData->listMetaTypes.constBegin(),
        coreMetaTypeStaticData->listMetaTypes.constEnd(),
        [&type](const McListMetaType &t) { return type == t.d->metaType; });
    if (itr == coreMetaTypeStaticData->listMetaTypes.constEnd()) {
        return McListMetaType();
    }
    return *itr;
}

QVector<McListMetaType> McListMetaType::metaTypes() noexcept
{
    return coreMetaTypeStaticData->listMetaTypes;
}

void McMapMetaType::registerMetaType(const McMapMetaType &type) noexcept
{
    if (!type.isValid() || type.d->isRegistered.loadRelaxed()) {
        return;
    }
    type.d->isRegistered.storeRelaxed(true);
#ifdef MC_USE_QT6
    type.d->metaType.id();
#endif
    coreMetaTypeStaticData->mapMetaTypes.append(type);
}

#ifdef MC_USE_QT5
McMapMetaType McMapMetaType::fromQMetaType(int type) noexcept
#else
McMapMetaType McMapMetaType::fromQMetaType(const QMetaType &type) noexcept
#endif
{
    auto itr = std::find_if(coreMetaTypeStaticData->mapMetaTypes.constBegin(),
        coreMetaTypeStaticData->mapMetaTypes.constEnd(),
        [&type](const McMapMetaType &t) { return type == t.d->metaType; });
    if (itr == coreMetaTypeStaticData->mapMetaTypes.constEnd()) {
        return McMapMetaType();
    }
    return *itr;
}

QVector<McMapMetaType> McMapMetaType::metaTypes() noexcept
{
    return coreMetaTypeStaticData->mapMetaTypes;
}
