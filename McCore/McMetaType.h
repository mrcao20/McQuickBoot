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

#include <QtCore/QMetaType>
#include <QtCore/QPointer>
#include <QtCore/QSharedPointer>
#include <QtCore/QVariant>
#include <QtCore/QWeakPointer>
#include <QtCore/QtGlobal>

#include "McMacroGlobal.h"
#include "Utils/IMcDestroyer.h"

#define MC_INTERFACES(...) \
 public: \
 using McPrivateTypeList = McPrivate::TypeList<__VA_ARGS__>; \
 using McPrivateTypeListHelper = void; \
\
 private:

inline bool operator<(const QVariant &left, const QVariant &right)
{
    auto res = QVariant::compare(left, right);
    return res == QPartialOrdering::Less;
}

class McMetaType;
namespace McPrivate {
class MetaTypeInterface
{
public:
    //! 是否已经保存在容器中
    mutable QAtomicInteger<bool> isRegistered;
    //! 原始类型
    QMetaType metaType;
    //! 指针类型
    QMetaType pMetaType;
    //! 智能指针类型
    QMetaType sMetaType;
    //! 弱指针类型
    QMetaType wMetaType;
    //! QPointer类型
    QMetaType tMetaType;
    //! 构造QSharedPointer的函数指针
    using CreateSharedPointerFn = QVariant (*)(void *);
    CreateSharedPointerFn createSharedPointer;
    //! 继承的父类的元数据
    mutable QVector<McMetaType> *parents;
};

class ListMetaTypeInterface
{
public:
    //! 是否已经保存在容器中
    mutable QAtomicInteger<bool> isRegistered;
    //! 列表自身类型
    QMetaType metaType;
    //! 容器内存储的数据类型
    QMetaType valueMetaType;
};

class MapMetaTypeInterface
{
public:
    //! 是否已经保存在容器中
    mutable QAtomicInteger<bool> isRegistered;
    //! 映射自身类型
    QMetaType metaType;
    //! 容器内键的数据类型
    QMetaType keyMetaType;
    //! 容器内值的数据类型
    QMetaType valueMetaType;
};

template<typename S>
class MetaTypeForType
{
public:
    static constexpr MetaTypeInterface::CreateSharedPointerFn getCreateSharedPointer() noexcept
    {
        if constexpr (std::is_default_constructible_v<S>) {
            return [](void *copy) {
                S *inPtr = static_cast<S *>(copy);
                if constexpr (std::is_base_of_v<IMcDestroyer, S>) {
                    if (inPtr == nullptr) {
                        inPtr = new S();
                    }
                    return QVariant::fromValue(QSharedPointer<S>(inPtr, &IMcDestroyer::destroy));
                } else {
                    if (inPtr == nullptr) {
                        return QVariant::fromValue(QSharedPointer<S>::create());
                    } else {
                        return QVariant::fromValue(QSharedPointer<S>(inPtr));
                    }
                }
            };
        } else {
            return nullptr;
        }
    }
};

template<typename T, int = QtPrivate::IsPointerToTypeDerivedFromQObject<T *>::Value ? QMetaType::PointerToQObject : 0>
struct MetaTypeInterfaceWrapper
{
    static inline constexpr const MetaTypeInterface metaType = {
        /*.isRegistered=*/false,
        /*.metaType=*/QMetaType::fromType<T>(),
        /*.pMetaType=*/QMetaType::fromType<T *>(),
        /*.sMetaType=*/QMetaType::fromType<QSharedPointer<T>>(),
        /*.wMetaType=*/QMetaType::fromType<QWeakPointer<T>>(),
        /*.tMetaType=*/QMetaType(),
        /*.createSharedPointer=*/MetaTypeForType<T>::getCreateSharedPointer(),
        /*.parents=*/nullptr,
    };
};

template<typename T>
struct MetaTypeInterfaceWrapper<T, QMetaType::PointerToQObject>
{
    static inline constexpr const MetaTypeInterface metaType = {
        /*.isRegistered=*/false,
        /*.metaType=*/QMetaType::fromType<T>(),
        /*.pMetaType=*/QMetaType::fromType<T *>(),
        /*.sMetaType=*/QMetaType::fromType<QSharedPointer<T>>(),
        /*.wMetaType=*/QMetaType::fromType<QWeakPointer<T>>(),
        /*.tMetaType=*/QMetaType::fromType<QPointer<T>>(),
        /*.createSharedPointer=*/MetaTypeForType<T>::getCreateSharedPointer(),
        /*.parents=*/nullptr,
    };
};

template<typename T>
struct ListMetaTypeInterfaceWrapper
{
    static inline constexpr const ListMetaTypeInterface metaType = {
        /*.isRegistered=*/false,
        /*.metaType=*/QMetaType::fromType<T>(),
        /*.valueMetaType=*/QMetaType::fromType<typename T::value_type>(),
    };
};

template<typename T>
struct MapMetaTypeInterfaceWrapper
{
    static inline constexpr const MapMetaTypeInterface metaType = {
        /*.isRegistered=*/false,
        /*.metaType=*/QMetaType::fromType<T>(),
        /*.keyMetaType=*/QMetaType::fromType<typename T::key_type>(),
        /*.valueMetaType=*/QMetaType::fromType<typename T::mapped_type>(),
    };
};

template<typename T>
constexpr const MetaTypeInterface *metaTypeInterfaceForType() noexcept
{
    using Ty = std::remove_cv_t<std::remove_reference_t<T>>;
    return &MetaTypeInterfaceWrapper<Ty>::metaType;
}

template<typename T>
constexpr const ListMetaTypeInterface *listMetaTypeInterfaceForType() noexcept
{
    using Ty = std::remove_cv_t<std::remove_reference_t<T>>;
    return &ListMetaTypeInterfaceWrapper<Ty>::metaType;
}

template<typename T>
constexpr const MapMetaTypeInterface *mapMetaTypeInterfaceForType() noexcept
{
    using Ty = std::remove_cv_t<std::remove_reference_t<T>>;
    return &MapMetaTypeInterfaceWrapper<Ty>::metaType;
}
} // namespace McPrivate

class MC_CORE_EXPORT McMetaType
{
public:
    constexpr McMetaType() noexcept = default;
    constexpr McMetaType(const McPrivate::MetaTypeInterface *val) noexcept
        : d(val)
    {
    }

    static void registerMetaType(const McMetaType &type) noexcept;

    static McMetaType fromQMetaType(const QMetaType &type) noexcept;
    static McMetaType fromPQMetaType(const QMetaType &type) noexcept;
    static McMetaType fromSQMetaType(const QMetaType &type) noexcept;
    static McMetaType fromWQMetaType(const QMetaType &type) noexcept;
    static McMetaType fromTQMetaType(const QMetaType &type) noexcept;
    static McMetaType fromTypeName(const QByteArray &typeName) noexcept;
    static McMetaType fromPTypeName(const QByteArray &typeName) noexcept;
    static McMetaType fromSTypeName(const QByteArray &typeName) noexcept;
    static McMetaType fromWTypeName(const QByteArray &typeName) noexcept;
    static McMetaType fromTTypeName(const QByteArray &typeName) noexcept;
    static QVector<McMetaType> metaTypes() noexcept;

    //! 原始类型
    constexpr QMetaType metaType() const noexcept
    {
        if (!isValid()) {
            return QMetaType();
        }
        return d->metaType;
    }
    //! 指针类型
    constexpr QMetaType pMetaType() const noexcept
    {
        if (!isValid()) {
            return QMetaType();
        }
        return d->pMetaType;
    }
    //! 智能指针类型
    constexpr QMetaType sMetaType() const noexcept
    {
        if (!isValid()) {
            return QMetaType();
        }
        return d->sMetaType;
    }
    //! 弱智能指针类型
    constexpr QMetaType wMetaType() const noexcept
    {
        if (!isValid()) {
            return QMetaType();
        }
        return d->wMetaType;
    }
    //! 跟踪指针类型
    constexpr QMetaType tMetaType() const noexcept
    {
        if (!isValid()) {
            return QMetaType();
        }
        return d->tMetaType;
    }

    QVariant createSharedPointer(void *copy = nullptr) noexcept;

    void addParentMetaType(const McMetaType &type) const noexcept;
    QVector<McMetaType> parentMetaTypes() const noexcept;

    constexpr bool isValid() const noexcept { return d != nullptr; }

    template<typename T>
    constexpr static McMetaType fromType()
    {
        return McMetaType(McPrivate::metaTypeInterfaceForType<T>());
    }

    friend bool operator==(McMetaType a, McMetaType b)
    {
        if (a.d == b.d)
            return true;
        if (a.d == nullptr || b.d == nullptr)
            return false;
        return a.d->metaType == b.d->metaType && a.d->pMetaType == b.d->pMetaType && a.d->sMetaType == b.d->sMetaType
               && a.d->wMetaType == b.d->wMetaType && a.d->tMetaType == b.d->tMetaType;
    }
    friend bool operator!=(McMetaType a, McMetaType b) { return !(a == b); }
    friend Q_DECL_CONST_FUNCTION size_t qHash(McMetaType key, size_t seed) noexcept
    {
        int id = 0;
        if (key.d != nullptr) {
            id = key.d->metaType.id();
        }
        return qHash(id, seed);
    }

private:
    const McPrivate::MetaTypeInterface *d{nullptr};
};

class MC_CORE_EXPORT McListMetaType
{
public:
    constexpr McListMetaType() noexcept = default;
    constexpr McListMetaType(const McPrivate::ListMetaTypeInterface *val) noexcept
        : d(val)
    {
    }

    static void registerMetaType(const McListMetaType &type) noexcept;

    static McListMetaType fromQMetaType(const QMetaType &type) noexcept;
    static QVector<McListMetaType> metaTypes() noexcept;

    ///! 列表自身类型
    constexpr QMetaType metaType() const noexcept
    {
        if (!isValid()) {
            return QMetaType();
        }
        return d->metaType;
    }
    //! 容器内存储的数据类型
    constexpr QMetaType valueMetaType() const noexcept
    {
        if (!isValid()) {
            return QMetaType();
        }
        return d->valueMetaType;
    }

    constexpr bool isValid() const noexcept { return d != nullptr; }

    template<typename T>
    constexpr static McListMetaType fromType()
    {
        return McListMetaType(McPrivate::listMetaTypeInterfaceForType<T>());
    }

    friend bool operator==(McListMetaType a, McListMetaType b)
    {
        if (a.d == b.d)
            return true;
        if (a.d == nullptr || b.d == nullptr)
            return false;
        return a.d->metaType == b.d->metaType && a.d->valueMetaType == b.d->valueMetaType;
    }
    friend bool operator!=(McListMetaType a, McListMetaType b) { return !(a == b); }

private:
    const McPrivate::ListMetaTypeInterface *d{nullptr};
};

class MC_CORE_EXPORT McMapMetaType
{
public:
    constexpr McMapMetaType() noexcept = default;
    constexpr McMapMetaType(const McPrivate::MapMetaTypeInterface *val) noexcept
        : d(val)
    {
    }

    static void registerMetaType(const McMapMetaType &type) noexcept;

    static McMapMetaType fromQMetaType(const QMetaType &type) noexcept;
    static QVector<McMapMetaType> metaTypes() noexcept;

    //! 映射自身类型
    constexpr QMetaType metaType() const noexcept
    {
        if (!isValid()) {
            return QMetaType();
        }
        return d->metaType;
    }
    //! 容器内键的数据类型
    constexpr QMetaType keyMetaType() const noexcept
    {
        if (!isValid()) {
            return QMetaType();
        }
        return d->keyMetaType;
    }
    //! 容器内值的数据类型
    constexpr QMetaType valueMetaType() const noexcept
    {
        if (!isValid()) {
            return QMetaType();
        }
        return d->valueMetaType;
    }

    constexpr bool isValid() const noexcept { return d != nullptr; }

    template<typename T>
    constexpr static McMapMetaType fromType()
    {
        return McMapMetaType(McPrivate::mapMetaTypeInterfaceForType<T>());
    }

    friend bool operator==(McMapMetaType a, McMapMetaType b)
    {
        if (a.d == b.d)
            return true;
        if (a.d == nullptr || b.d == nullptr)
            return false;
        return a.d->metaType == b.d->metaType && a.d->keyMetaType == b.d->keyMetaType
               && a.d->valueMetaType == b.d->valueMetaType;
    }
    friend bool operator!=(McMapMetaType a, McMapMetaType b) { return !(a == b); }

private:
    const McPrivate::MapMetaTypeInterface *d{nullptr};
};

namespace McPrivate {
template<typename...>
struct TypeList;

template<typename T, typename... U>
struct TypeList<T, U...>
{
    using Head = T;
    using Tails = TypeList<U...>;
};

// 针对空list的特化
template<>
struct TypeList<>
{
};

template<typename From, typename To>
struct RegisterConverterHelper2
{
    using FromPtr = QSharedPointer<From>;
    using ToPtr = QSharedPointer<To>;
    static void registerConverter() noexcept
    {
        if (!QMetaType::hasRegisteredConverterFunction<FromPtr, ToPtr>()) {
            QMetaType::registerConverter<FromPtr, ToPtr>();
        }
        if constexpr (!bool(QtPrivate::IsPointerToTypeDerivedFromQObject<From *>::Value)
                      || !bool(QtPrivate::IsPointerToTypeDerivedFromQObject<To *>::Value)) {
            if (!QMetaType::hasRegisteredConverterFunction<From *, To *>()) {
                QMetaType::registerConverter<From *, To *>();
            }
        }
        if (!QMetaType::hasRegisteredConverterFunction<FromPtr, To *>()) {
            QMetaType::registerConverter<FromPtr, To *>([](const FromPtr &from) { return from.operator->(); });
        }
        constexpr McMetaType srcMetaType = McMetaType::fromType<From>();
        constexpr McMetaType dstMetaType = McMetaType::fromType<To>();
        srcMetaType.addParentMetaType(dstMetaType);
    }
};

template<typename From, typename To, typename Enable = void>
struct RegisterConverterHelper
{
    static void registerConverter() noexcept { RegisterConverterHelper2<From, To>::registerConverter(); }
};

template<typename From, typename To>
struct RegisterConverterHelper<From, To, typename To::McPrivateTypeListHelper>
{
    static void registerConverter() noexcept
    {
        RegisterConverterHelper2<From, To>::registerConverter();
        RegisterConverterHelper<From, typename To::McPrivateTypeList>::registerConverter();
    }
};

template<typename From, typename... Tos>
struct RegisterConverterHelper<From, TypeList<Tos...>, void>
{
    static void registerConverter() noexcept
    {
        RegisterConverterHelper<From, typename TypeList<Tos...>::Head>::registerConverter();
        RegisterConverterHelper<From, typename TypeList<Tos...>::Tails>::registerConverter();
    }
};

template<typename From>
struct RegisterConverterHelper<From, QObject, void>
{
    static void registerConverter() noexcept {}
};

template<typename From>
struct RegisterConverterHelper<From, TypeList<>, void>
{
    static void registerConverter() noexcept {}
};

template<typename T>
struct MetaTypeRegister2
{
    static void registerMetaType() noexcept
    {
        using TPtr = QSharedPointer<T>;
        if (!QMetaType::hasRegisteredConverterFunction<TPtr, T *>()) {
            QMetaType::registerConverter<TPtr, T *>([](const TPtr &from) { return from.operator->(); });
        }
        if constexpr (bool(QtPrivate::IsGadgetHelper<T>::IsRealGadget)) {
            if (!QMetaType::hasRegisteredConverterFunction<T *, TPtr>()) {
                QMetaType::registerConverter<T *, TPtr>([](T *from) { return QSharedPointer<T>(from); });
            }
        } else if constexpr (bool(QtPrivate::IsPointerToTypeDerivedFromQObject<T *>::Value)) {
            using QObjectPtr = QSharedPointer<QObject>;
            if (!QMetaType::hasRegisteredConverterFunction<TPtr, QObjectPtr>()) {
                QMetaType::registerConverter<TPtr, QObjectPtr>();
            }
            if (!QMetaType::hasRegisteredConverterFunction<QObjectPtr, TPtr>()) {
                QMetaType::registerConverter<QObjectPtr, TPtr>(
                    [](const QObjectPtr &from) { return from.template objectCast<T>(); });
            }
        }
    }
};

template<typename T, typename Enable = void>
struct MetaTypeRegister
{
    static void registerMetaType() noexcept { MetaTypeRegister2<T>::registerMetaType(); }
};

template<typename T>
struct MetaTypeRegister<T, typename T::McPrivateTypeListHelper>
{
    static void registerMetaType() noexcept
    {
        MetaTypeRegister2<T>::registerMetaType();
        using To = typename T::McPrivateTypeList;
        RegisterConverterHelper<T, To>::registerConverter();
    }
};
} // namespace McPrivate

template<typename T>
inline void mcRegisterMetaTypeSimple() noexcept
{
    static_assert(!std::is_pointer<T>::value, "mcRegisterMetaTypeSimple's template type must not be a pointer type");
    constexpr McMetaType metaType = McMetaType::fromType<T>();
    metaType.metaType().id();
    metaType.pMetaType().id();
    metaType.sMetaType().id();
    McMetaType::registerMetaType(metaType);
}

template<typename T>
inline void mcRegisterMetaType() noexcept
{
    static_assert(!std::is_pointer<T>::value, "mcRegisterMetaType's template type must not be a pointer type");
    mcRegisterMetaTypeSimple<T>();
    McPrivate::MetaTypeRegister<T>::registerMetaType();
}

template<typename T>
inline void mcRegisterContainerConverter() noexcept
{
    constexpr QMetaType metaType = QMetaType::fromType<T>();
    if constexpr (bool(QtPrivate::IsSequentialContainer<T>::Value)) {
        using ValueType = typename T::value_type;
        mcRegisterContainerConverter<ValueType>();
        constexpr McListMetaType customMetaType = McListMetaType::fromType<T>();
        McListMetaType::registerMetaType(customMetaType);
        constexpr QMetaType listMetaType = QMetaType::fromType<QVariantList>();
        if (!QMetaType::hasRegisteredConverterFunction(listMetaType, metaType)) {
            QVariant var(listMetaType);
            if (!var.canConvert(metaType)) {
                QMetaType::registerConverter<QVariantList, T>([](const QVariantList &from) {
                    T to;
                    for (const auto &f : from) {
                        to << f.template value<typename T::value_type>();
                    }
                    return to;
                });
            }
        }
        if (!QMetaType::hasRegisteredConverterFunction(metaType, listMetaType)) {
            QVariant var(metaType);
            if (!var.canConvert(listMetaType)) {
                QMetaType::registerConverter<T, QVariantList>([](const T &from) {
                    QVariantList to;
                    for (const auto &f : from) {
                        to << QVariant::fromValue(f);
                    }
                    return to;
                });
            }
        }
    } else if constexpr (bool(QtPrivate::IsAssociativeContainer<T>::Value)) {
        using KeyType = typename T::key_type;
        using ValueType = typename T::mapped_type;
        mcRegisterContainerConverter<KeyType>();
        mcRegisterContainerConverter<ValueType>();
        constexpr McMapMetaType customMetaType = McMapMetaType::fromType<T>();
        McMapMetaType::registerMetaType(customMetaType);
        constexpr QMetaType mapMetaType = QMetaType::fromType<QMap<QVariant, QVariant>>();
        if (!QMetaType::hasRegisteredConverterFunction(mapMetaType, metaType)) {
            QVariant var(mapMetaType);
            if (!var.canConvert(metaType)) {
                QMetaType::registerConverter<QMap<QVariant, QVariant>, T>([](const QMap<QVariant, QVariant> &from) {
                    T to;
                    using keyType = typename T::key_type;
                    using mappedType = typename T::mapped_type;
                    for (auto itr = from.cbegin(), end = from.cend(); itr != end; ++itr) {
                        keyType key = itr.key().template value<keyType>();
                        mappedType value = itr.value().template value<mappedType>();
                        to.insert(key, value);
                    }
                    return to;
                });
            }
        }
        if (!QMetaType::hasRegisteredConverterFunction(metaType, mapMetaType)) {
            QVariant var(metaType);
            if (!var.canConvert(mapMetaType)) {
                QMetaType::registerConverter<T, QMap<QVariant, QVariant>>([](const T &from) {
                    QMap<QVariant, QVariant> to;
                    for (auto itr = from.cbegin(), end = from.cend(); itr != end; ++itr) {
                        to.insert(QVariant::fromValue(itr.key()), QVariant::fromValue(itr.value()));
                    }
                    return to;
                });
            }
        }
    }
}
