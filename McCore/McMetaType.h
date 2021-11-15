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
#include <QtCore/QtGlobal>

#include "McMacroGlobal.h"

#define MC_TYPELIST(...) \
public: \
    using McPrivateTypeList = McPrivate::TypeList<__VA_ARGS__>; \
    using McPrivateTypeListHelper = void; \
\
private:

#define MC_INTERFACES(...) MC_TYPELIST(__VA_ARGS__)

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
    //! 继承的父类的元数据
    mutable QVector<McMetaType> *parents;
};

template<typename T>
struct MetaTypeInterfaceWrapper
{
    static inline constexpr const MetaTypeInterface metaType = {
        /*.isRegistered=*/false,
        /*.metaType=*/QMetaType::fromType<T>(),
        /*.pMetaType=*/QMetaType::fromType<T *>(),
        /*.sMetaType=*/QMetaType::fromType<QSharedPointer<T>>(),
        /*.parents=*/nullptr,
    };
};

template<typename T>
constexpr const MetaTypeInterface *metaTypeInterfaceForType() noexcept
{
    using Ty = std::remove_cv_t<std::remove_reference_t<T>>;
    return &MetaTypeInterfaceWrapper<Ty>::metaType;
}
} // namespace McPrivate

class MC_CORE_EXPORT McMetaType
{
public:
    constexpr McMetaType() noexcept = default;
    constexpr McMetaType(const McPrivate::MetaTypeInterface *val) noexcept : d(val) {}

    static void registerMetaType(const McMetaType &type) noexcept;

    static McMetaType fromQMetaType(const QMetaType &type) noexcept;
    static McMetaType fromPQMetaType(const QMetaType &type) noexcept;
    static McMetaType fromSQMetaType(const QMetaType &type) noexcept;
    static McMetaType fromTypeName(const QByteArray &typeName) noexcept;
    static McMetaType fromPTypeName(const QByteArray &typeName) noexcept;
    static McMetaType fromSTypeName(const QByteArray &typeName) noexcept;
    static QVector<McMetaType> metaTypes() noexcept;

    //! 原始类型
    QMetaType metaType() const noexcept;
    //! 指针类型
    QMetaType pMetaType() const noexcept;
    //! 智能指针类型
    QMetaType sMetaType() const noexcept;

    void addParentMetaType(const McMetaType &type) const noexcept;
    QVector<McMetaType> parentMetaTypes() const noexcept;

    bool isValid() const noexcept { return d != nullptr; }

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
        return a.d->metaType == b.d->metaType && a.d->pMetaType == b.d->pMetaType && a.d->sMetaType == b.d->sMetaType;
    }
    friend bool operator!=(McMetaType a, McMetaType b) { return !(a == b); }

private:
    const McPrivate::MetaTypeInterface *d{nullptr};
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
{};

template<typename From, typename To>
To sharedPointerObjectConverter(const From &from) noexcept
{
    return from.template objectCast<typename To::Type>();
}

template<typename T>
QSharedPointer<T> gadgetConverter(T *from) noexcept
{
    return QSharedPointer<T>(from);
}

template<typename From, typename To>
To sharedToStar(const From &from) noexcept
{
    return from.operator->();
}

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
            QMetaType::registerConverter<FromPtr, To *>(sharedToStar<FromPtr, To *>);
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
            QMetaType::registerConverter<TPtr, T *>(sharedToStar<TPtr, T *>);
        }
        if constexpr (bool(QtPrivate::IsGadgetHelper<T>::IsRealGadget)) {
            if (!QMetaType::hasRegisteredConverterFunction<T *, TPtr>()) {
                QMetaType::registerConverter<T *, TPtr>(gadgetConverter<T>);
            }
        } else if (bool(QtPrivate::IsPointerToTypeDerivedFromQObject<T *>::Value)) {
            using QObjectPtr = QSharedPointer<QObject>;
            if (!QMetaType::hasRegisteredConverterFunction<TPtr, QObjectPtr>()) {
                QMetaType::registerConverter<TPtr, QObjectPtr>();
            }
            if (!QMetaType::hasRegisteredConverterFunction<QObjectPtr, TPtr>()) {
                QMetaType::registerConverter<QObjectPtr, TPtr>(sharedPointerObjectConverter<QObjectPtr, TPtr>);
            }
        }
        constexpr McMetaType metaType = McMetaType::fromType<T>();
        McMetaType::registerMetaType(metaType);
        QByteArray typeName;
        typeName.append("QSharedPointer<").append(metaType.metaType().name()).append('>');
        qRegisterMetaType<TPtr>(typeName);
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
inline void mcRegisterMetaType() noexcept
{
    static_assert(!std::is_pointer<T>::value, "mcRegisterMetaType's template type must not be a pointer type");
    McPrivate::MetaTypeRegister<T>::registerMetaType();
}
