#pragma once

#include <QMetaType>
#include <QVariant>

#include "../McMacroGlobal.h"
#include "impl/McMetaTypeId.h"

#define MC_DECL_METATYPE(Class) \
    MC_DECL_POINTER(Class) \
    Q_DECLARE_METATYPE(Class##Ptr) \
    Q_DECLARE_METATYPE(Class*)

#define MC_DECL_INIT(Class) \
    static const int Class##_Static_Init;

#define MC_INIT(Class) \
    const int Class::Class##_Static_Init = []() -> int {

#define MC_INIT_END \
    return 0;}();

#define MC_DEFINE_TYPELIST(...) \
public: \
    using McPrivateTypeList = McPrivate::McTypeList<__VA_ARGS__>; \
private:

#define MC_DECL_TYPELIST(Class) \
    Class, Class::McPrivateTypeList

#define MC_TYPELIST(Class) MC_DECL_TYPELIST(Class)

#define MC_FIRST_TYPE_NAME(Class, ...) MC_MACRO_STR(Class)
#define MC_FIRST_TYPE_NAME_CONST_REF(Class, ...) MC_MACRO_STR(Class##ConstPtrRef)
#define MC_REGISTER_BEAN_FACTORY(...) MC_REGISTER_BEAN_FACTORY_IMPL(__VA_ARGS__)
#define MC_REGISTER_BEAN_FACTORY_IMPL(...) \
    mcRegisterBeanFactory<__VA_ARGS__>(MC_FIRST_TYPE_NAME(__VA_ARGS__), MC_FIRST_TYPE_NAME_CONST_REF(__VA_ARGS__));

#define MC_REGISTER_LIST_CONVERTER(Type) \
    mcRegisterListConverter<Type>(#Type);
#define MC_REGISTER_MAP_CONVERTER(Type) \
    mcRegisterMapConverter<Type>(#Type);


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
    McMetaTypeId::addQObjectPointerIds(qRegisterMetaType<T*>(typeName));
    McMetaTypeId::addSharedPointerId(qRegisterMetaType<TPtr>(constRefTypeName));
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
To mcConverterList(const From &from) 
{
	To to;
	for (const auto &f : from) {
        to << f.template value<typename To::value_type>();
	}
	return to;
}

template<typename From, typename To>
To mcConverterMap(const From &from) 
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
        auto className = ObjType::staticMetaObject.className();
        mcRegisterBeanFactory<ObjType>(className, QString("%1ConstPtrRef").arg(className).toLocal8Bit());
        return QMetaType::type(className);
    }
};

template<typename T>
struct McMetaTypeIdHelper<T, QMetaType::PointerToQObject>
{
    static int metaTypeId()
    {
        typedef typename std::remove_pointer<T>::type ObjType;
        auto className = ObjType::staticMetaObject.className();
        mcRegisterBeanFactory<ObjType>(className, QString("%1ConstPtrRef").arg(className).toLocal8Bit());
        return QMetaType::type(className);
    }
};

}

template<typename From, typename To>
void mcRegisterListConverter() 
{
	if (QMetaType::hasRegisteredConverterFunction(qMetaTypeId<From>(), qMetaTypeId<To>()))
		return;
    QVariant var;
    var.setValue(From());
    if(var.canConvert<To>()) {
        return;
    }
	QMetaType::registerConverter<From, To>(McPrivate::mcConverterList<From, To>);
}

template<typename T>
void mcRegisterListConverter(const char *typeName)
{
    auto id = QMetaType::type(typeName);
    if(id == QMetaType::UnknownType) {
        id = qRegisterMetaType<T>(typeName);
    }
    typedef typename T::value_type ValueType;
    int valueId = McPrivate::McMetaTypeIdHelper<ValueType>::metaTypeId();
    McMetaTypeId::addSequentialId(id, valueId);
	mcRegisterListConverter<QVariantList, T>();
}

template<typename From, typename To>
void mcRegisterMapConverter()
{
	if (QMetaType::hasRegisteredConverterFunction(qMetaTypeId<From>(), qMetaTypeId<To>()))
		return;
    QVariant var;
    var.setValue(From());
    if(var.canConvert<To>()) {
        return;
    }
	QMetaType::registerConverter<From, To>(McPrivate::mcConverterMap<From, To>);
}

template<typename T>
void mcRegisterMapConverter(const char *typeName)
{
    auto id = QMetaType::type(typeName);
    if(id == QMetaType::UnknownType) {
        id = qRegisterMetaType<T>(typeName);
    }
    typedef typename T::key_type KeyType;
    typedef typename T::mapped_type ValueType;
    int keyId = McPrivate::McMetaTypeIdHelper<KeyType>::metaTypeId();
    int valueId = McPrivate::McMetaTypeIdHelper<ValueType>::metaTypeId();
    McMetaTypeId::addAssociativeId(id, keyId, valueId);
	mcRegisterMapConverter<QMap<QVariant, QVariant>, T>();
}
