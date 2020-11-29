#pragma once

#include <QCoreApplication>
#include <QMetaType>
#include <QVariant>

#include "../McMacroGlobal.h"
#include "impl/McMetaTypeId.h"

#define MC_DECL_METATYPE(Class) \
    MC_DECL_POINTER(Class) \
    Q_DECLARE_METATYPE(Class##Ptr) \
    Q_DECLARE_METATYPE(Class*)

#define MC_DEFINE_TYPELIST(...) \
public: \
    using McPrivateTypeList = McPrivate::McTypeList<__VA_ARGS__>; \
private:

#define MC_DECL_TYPELIST(Class) \
    Class, Class::McPrivateTypeList

#define MC_TYPELIST(Class) MC_DECL_TYPELIST(Class)

#ifdef Q_CC_MSVC
# define MC_FIRST_TYPE_NAME(par) MC_FIRST_TYPE_NAME_I par
# define MC_FIRST_TYPE_NAME_CONST_REF(par) MC_FIRST_TYPE_NAME_CONST_REF_I par
# define MC_FIRST_TYPE_NAME_I(Class, ...) MC_MACRO_STR(Class)
# define MC_FIRST_TYPE_NAME_CONST_REF_I(Class, ...) MC_MACRO_STR(Class##ConstPtrRef)
# define MC_REGISTER_BEAN_FACTORY(par) MC_REGISTER_BEAN_FACTORY_IMPL(MC_FIRST_TYPE_NAME((par)), MC_FIRST_TYPE_NAME_CONST_REF((par)), par)
# define MC_REGISTER_BEAN_FACTORY_IMPL(CN, CCRN, ...) \
    mcRegisterBeanFactory<__VA_ARGS__>(CN, CCRN);
#else
# define MC_FIRST_TYPE_NAME(Class, ...) MC_MACRO_STR(Class)
# define MC_FIRST_TYPE_NAME_CONST_REF(Class, ...) MC_MACRO_STR(Class##ConstPtrRef)
# define MC_REGISTER_BEAN_FACTORY(...) MC_REGISTER_BEAN_FACTORY_IMPL(__VA_ARGS__)
# define MC_REGISTER_BEAN_FACTORY_IMPL(...) \
    mcRegisterBeanFactory<__VA_ARGS__>(MC_FIRST_TYPE_NAME(__VA_ARGS__), MC_FIRST_TYPE_NAME_CONST_REF(__VA_ARGS__));
#endif

#define MC_REGISTER_CONTAINER_CONVERTER(Type) \
    McPrivate::McContainerConverterRegisterHelper<Type>::registerConverter(#Type);
#define MC_REGISTER_LIST_CONVERTER MC_REGISTER_CONTAINER_CONVERTER
#define MC_REGISTER_MAP_CONVERTER MC_REGISTER_CONTAINER_CONVERTER

namespace McPrivate {

template <typename...> struct McTypeList;

template <typename T, typename... U>
struct McTypeList<T, U...> 
{
    using Head = T;
    using Tails = McTypeList<U...>;
};

// 针对空list的特化
template <>
struct McTypeList<> {};

template<typename From, typename To>
struct McRegisterConverterHelper 
{
    using FromPtr = QSharedPointer<From>;
    using ToPtr = QSharedPointer<To>;
    static void registerConverter()
    {
        if (!QMetaType::hasRegisteredConverterFunction<FromPtr, ToPtr>()) {
            QMetaType::registerConverter<FromPtr, ToPtr>();
        }
    }
};

template<typename From, typename... Tos>
struct McRegisterConverterHelper<From, McPrivate::McTypeList<Tos...>> 
{
    static void registerConverter()
    {
        using TypeList = McPrivate::McTypeList<Tos...>;
        McRegisterConverterHelper<From, typename TypeList::Head>::registerConverter();
        McRegisterConverterHelper<From, typename TypeList::Tails>::registerConverter();
    }
};

template<typename From>
struct McRegisterConverterHelper<From, QObject> 
{
    static void registerConverter()
    {}
};

template<typename From>
struct McRegisterConverterHelper<From, McPrivate::McTypeList<>> 
{
    static void registerConverter()
    {}
};

}

template<typename From, typename To>
To mcConverterQSharedPointerObject(const From &from) 
{
    return from.template objectCast<typename To::Type>();
}

template<typename T>
void mcRegisterBeanFactory(const char *typeName, const char *constRefTypeName)
{
    Q_ASSERT(typeName != nullptr && constRefTypeName != nullptr);
    using TPtr = QSharedPointer<T>;
    using QObjectPtr = QSharedPointer<QObject>;
    Q_STATIC_ASSERT_X(!std::is_pointer<T>::value, "mcRegisterBeanFactory's template type must not be a pointer type");
    if (!QMetaType::hasRegisteredConverterFunction<TPtr, QObjectPtr>()) {
        QMetaType::registerConverter<TPtr, QObjectPtr>();
    }
    if (!QMetaType::hasRegisteredConverterFunction<QObjectPtr, TPtr>()) {
        QMetaType::registerConverter<QObjectPtr, TPtr>(mcConverterQSharedPointerObject<QObjectPtr, TPtr>);
    }
    auto pId = qRegisterMetaType<T*>(typeName);
    auto sId = qRegisterMetaType<TPtr>(constRefTypeName);
    McMetaTypeId::addQObjectPointerIds(pId, sId);
    McMetaTypeId::addSharedPointerId(sId, pId);
}

template<typename From, typename To>
void mcRegisterBeanFactory(const char *typeName, const char *constRefTypeName) 
{
    mcRegisterBeanFactory<From>(typeName, constRefTypeName);
    Q_STATIC_ASSERT_X(!std::is_pointer<To>::value, "mcRegisterBeanFactory's template type must not be a pointer type");
    McPrivate::McRegisterConverterHelper<From, To>::registerConverter();
}

namespace McPrivate {

template<typename T>
struct TypeSelector 
{
    typedef T BaseType; 
    typedef QSharedPointer<T> Type;
};
template<typename T>
struct TypeSelector<T *> 
{
    typedef T BaseType;
    typedef QSharedPointer<T> Type;
};
template<typename T>
struct TypeSelector<QSharedPointer<T>> 
{
    typedef T BaseType;
    typedef QSharedPointer<T> Type;
};

template<typename From, typename To>
struct McConverterList
{
    static To registerConverter(const From &from)
    {
        Q_UNUSED(from);
    }
};

template<typename To>
struct McConverterList<QVariantList, To>
{
    static To registerConverter(const QVariantList &from)
    {
        To to;
        for (const auto &f : from) {
            to << f.template value<typename To::value_type>();
        }
        return to;
    }
};

template<typename From>
struct McConverterList<From, QVariantList>
{
    static QVariantList registerConverter(const From &from)
    {
        QVariantList to;
        for (const auto &f : from) {
            to << QVariant::fromValue(f);
        }
        return to;
    }
};

template<typename From, typename To>
struct McConverterMap
{
    static To registerConverter(const From &from)
    {
        Q_UNUSED(from);
    }
};

template<typename To>
struct McConverterMap<QMap<QVariant, QVariant>, To>
{
    static To registerConverter(const QMap<QVariant, QVariant> &from)
    {
        To to;
        using keyType = typename To::key_type;
        using mappedType = typename To::mapped_type;
        for (auto itr = from.cbegin(); itr != from.cend(); ++itr) {
            keyType key = itr.key().template value<keyType>();
            mappedType value = itr.value().template value<mappedType>();
            to.insert(key, value);
        }
        return to;
    }
};

template<typename From>
struct McConverterMap<From, QMap<QVariant, QVariant>>
{
    static QMap<QVariant, QVariant> registerConverter(const From &from)
    {
        QMap<QVariant, QVariant> to;
        for (auto itr = from.cbegin(); itr != from.cend(); ++itr) {
            to.insert(QVariant::fromValue(itr.key()), QVariant::fromValue(itr.value()));
        }
        return to;
    }
};

template<typename T, int =
    QtPrivate::IsSharedPointerToTypeDerivedFromQObject<T>::Value ? QMetaType::SharedPointerToQObject :
    QtPrivate::IsPointerToTypeDerivedFromQObject<T>::Value ? QMetaType::PointerToQObject : 0>
struct McMetaTypeIdHelper
{
    static int metaTypeId()
    {
        return qMetaTypeId<T>();
    }
};

template<typename T>
struct McMetaTypeIdHelper<T, QMetaType::SharedPointerToQObject>
{
    static int metaTypeId()
    {
        typedef typename T::Type ObjType;
        return qMetaTypeId<ObjType *>();
    }
};

template<typename T>
struct McMetaTypeIdHelper<T, QMetaType::PointerToQObject>
{
    static int metaTypeId()
    {
        typedef typename std::remove_pointer<T>::type ObjType;
        return qMetaTypeId<ObjType *>();
    }
};

}

namespace McPrivate {

template<typename From, typename To>
struct McListConverterRegisterHelper
{
    static void registerConverter()
    {
        if (QMetaType::hasRegisteredConverterFunction(qMetaTypeId<From>(), qMetaTypeId<To>()))
            return;
        QVariant var;
        var.setValue(From());
        if(var.canConvert<To>()) {
            return;
        }
        QMetaType::registerConverter<From, To>(McPrivate::McConverterList<From, To>::registerConverter);
    }
};

template<typename From, typename To>
struct McMapConverterRegisterHelper
{
    static void registerConverter()
    {
        if (QMetaType::hasRegisteredConverterFunction(qMetaTypeId<From>(), qMetaTypeId<To>()))
            return;
        QVariant var;
        var.setValue(From());
        if(var.canConvert<To>()) {
            return;
        }
        QMetaType::registerConverter<From, To>(McPrivate::McConverterMap<From, To>::registerConverter);
    }
};

template<typename T, int = 
         QtPrivate::IsSequentialContainer<T>::Value ? 1 :
         QtPrivate::IsAssociativeContainer<T>::Value ? 2 : 0>
struct McContainerConverterRegisterHelper
{
    static void registerConverter(const char *typeName)
    {
        Q_UNUSED(typeName)
    }
};

template<typename T>
struct McContainerConverterRegisterHelper<T, 1>
{
    static void registerConverter(const char *typeName)
    {
        auto id = QMetaType::type(typeName);
        if(id == QMetaType::UnknownType) {
            id = qRegisterMetaType<T>(typeName);
        }
        typedef typename T::value_type ValueType;
        int valueId = McPrivate::McMetaTypeIdHelper<ValueType>::metaTypeId();
        McContainerConverterRegisterHelper<ValueType>::registerConverter(QMetaType::typeName(valueId));
        McMetaTypeId::addSequentialId(id, valueId);
        McListConverterRegisterHelper<QVariantList, T>::registerConverter();
        McListConverterRegisterHelper<T, QVariantList>::registerConverter();
    }
};

template<typename T>
struct McContainerConverterRegisterHelper<T, 2>
{
    static void registerConverter(const char *typeName)
    {
        auto id = QMetaType::type(typeName);
        if(id == QMetaType::UnknownType) {
            id = qRegisterMetaType<T>(typeName);
        }
        typedef typename T::key_type KeyType;
        typedef typename T::mapped_type ValueType;
        int keyId = McPrivate::McMetaTypeIdHelper<KeyType>::metaTypeId();
        int valueId = McPrivate::McMetaTypeIdHelper<ValueType>::metaTypeId();
        McContainerConverterRegisterHelper<KeyType>::registerConverter(QMetaType::typeName(keyId));
        McContainerConverterRegisterHelper<ValueType>::registerConverter(QMetaType::typeName(valueId));
        McMetaTypeId::addAssociativeId(id, keyId, valueId);
        McMapConverterRegisterHelper<QMap<QVariant, QVariant>, T>::registerConverter();
        McMapConverterRegisterHelper<T, QMap<QVariant, QVariant>>::registerConverter();
    }
};

}
