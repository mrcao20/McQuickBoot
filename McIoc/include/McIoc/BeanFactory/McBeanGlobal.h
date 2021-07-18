#pragma once

#include <QCoreApplication>
#include <QMetaType>
#include <QVariant>

#include "../ObjectBuilder/ObjectBuilder.h"
#include "impl/McMetaTypeId.h"

#define MC_TYPELIST(...) \
public: \
    using McPrivateTypeList = McPrivate::McTypeList<__VA_ARGS__>; \
    using McPrivateTypeListHelper = void; \
\
private:

#define MC_INTERFACES(...) MC_TYPELIST(__VA_ARGS__)

#define MC_REGISTER_BEAN_FACTORY(Class, ...) \
    McPrivate::McConverterRegister<Class, ##__VA_ARGS__>::registerConverter();

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
template <> struct McTypeList<> {};

template<typename From, typename To>
To mcConverterQSharedPointerObject(const From &from)
{
    return from.template objectCast<typename To::Type>();
}

template<typename T>
QSharedPointer<T> mcConverterGadget(T *from)
{
    return QSharedPointer<T>(from);
}

template<typename From, typename To>
To mcSharedToStar(const From &from)
{
    return from.operator->();
}

template<typename From, typename To>
struct McRegisterConverterHelper2
{
    using FromPtr = QSharedPointer<From>;
    using ToPtr = QSharedPointer<To>;
    static void registerConverter(int metaId)
    {
        if (!QMetaType::hasRegisteredConverterFunction<FromPtr, ToPtr>()) {
            QMetaType::registerConverter<FromPtr, ToPtr>();
        }
        if (!QMetaType::hasRegisteredConverterFunction<From *, To *>()) {
            QMetaType::registerConverter<From *, To *>();
        }
        if (!QMetaType::hasRegisteredConverterFunction<FromPtr, To *>()) {
            QMetaType::registerConverter<FromPtr, To *>(mcSharedToStar<FromPtr, To *>);
        }
        auto dstMetaId = qMetaTypeId<ToPtr>();
        McMetaTypeId::addMetaIdMap(metaId, dstMetaId);
    }
};

template<typename From, typename To, typename Enable = void>
struct McRegisterConverterHelper
{
    static void registerConverter(int metaId)
    {
        McRegisterConverterHelper2<From, To>::registerConverter(metaId);
    }
};

template<typename From, typename To>
struct McRegisterConverterHelper<From, To, typename To::McPrivateTypeListHelper>
{
    static void registerConverter(int metaId)
    {
        McRegisterConverterHelper2<From, To>::registerConverter(metaId);
        McRegisterConverterHelper<From, typename To::McPrivateTypeList>::registerConverter(metaId);
    }
};

template<typename From, typename... Tos>
struct McRegisterConverterHelper<From, McPrivate::McTypeList<Tos...>, void>
{
    static void registerConverter(int metaId)
    {
        using TypeList = McPrivate::McTypeList<Tos...>;
        McRegisterConverterHelper<From, typename TypeList::Head>::registerConverter(metaId);
        McRegisterConverterHelper<From, typename TypeList::Tails>::registerConverter(metaId);
    }
};

template<typename From>
struct McRegisterConverterHelper<From, QObject, void>
{
    static void registerConverter(int) {}
};

template<typename From>
struct McRegisterConverterHelper<From, McPrivate::McTypeList<>, void>
{
    static void registerConverter(int) {}
};

template<typename T, bool flag, typename Enable = void>
struct McRegisterBeanFactory
{
    static void registerBeanFactory()
    {
        using TPtr = QSharedPointer<T>;
        if (!QMetaType::hasRegisteredConverterFunction<TPtr, T *>()) {
            QMetaType::registerConverter<TPtr, T *>(mcSharedToStar<TPtr, T *>);
        }
        auto pId = qRegisterMetaType<T *>();
        qRegisterMetaType<TPtr>();

        QByteArray cName = QMetaType::typeName(pId);
        Q_ASSERT(!cName.isEmpty());
        cName.remove(cName.length() - 1, 1);
        QByteArray typeName;
        typeName.append("QSharedPointer<").append(cName).append('>');
        qRegisterMetaType<TPtr>(typeName);

        typeName.clear();
        typeName.reserve(int(strlen(cName)) + 11);
        typeName.append(cName).append("ConstPtrRef");
        qRegisterMetaType<TPtr>(typeName);
    }
};

template<typename T, bool flag>
struct McRegisterBeanFactory<T,
                             flag,
                             typename std::enable_if<QtPrivate::IsGadgetHelper<T>::IsRealGadget>::type>
{
    static void registerBeanFactory()
    {
        using TPtr = QSharedPointer<T>;
        if (!QMetaType::hasRegisteredConverterFunction<T *, TPtr>()) {
            QMetaType::registerConverter<T *, TPtr>(mcConverterGadget<T>);
        }
        if (!QMetaType::hasRegisteredConverterFunction<TPtr, T *>()) {
            QMetaType::registerConverter<TPtr, T *>(mcSharedToStar<TPtr, T *>);
        }
        auto gId = qRegisterMetaType<T>();
        auto pId = qRegisterMetaType<T *>();
        auto sId = qRegisterMetaType<TPtr>();

        McMetaTypeId::addGadget(gId, pId, sId);

        const char *const cName = T::staticMetaObject.className();
        QByteArray typeName;
        typeName.append("QSharedPointer<").append(cName).append('>');
        qRegisterMetaType<TPtr>(typeName);

        typeName.clear();
        typeName.reserve(int(strlen(cName)) + 11);
        typeName.append(cName).append("ConstPtrRef");
        qRegisterMetaType<TPtr>(typeName);
    }
};

template<typename T, bool flag>
struct AddQObjectBuilderHelper
{
    static void addQObjectBuilder(int) {}
};
template<typename T>
struct AddQObjectBuilderHelper<T, true>
{
    static void addQObjectBuilder(int id)
    {
        auto builder = QObjectBuilderPtr<std::remove_pointer_t<T>>::create();
        IQObjectBuilder::addQObjectBuilder(id, builder);
    }
};

template<typename T, bool flag>
struct McRegisterBeanFactory<
    T,
    flag,
    typename std::enable_if<QtPrivate::IsPointerToTypeDerivedFromQObject<T *>::Value>::type>
{
    static void registerBeanFactory()
    {
        using TPtr = QSharedPointer<T>;
        using QObjectPtr = QSharedPointer<QObject>;
        if (!QMetaType::hasRegisteredConverterFunction<TPtr, QObjectPtr>()) {
            QMetaType::registerConverter<TPtr, QObjectPtr>();
        }
        if (!QMetaType::hasRegisteredConverterFunction<QObjectPtr, TPtr>()) {
            QMetaType::registerConverter<QObjectPtr, TPtr>(
                mcConverterQSharedPointerObject<QObjectPtr, TPtr>);
        }
        if (!QMetaType::hasRegisteredConverterFunction<TPtr, T *>()) {
            QMetaType::registerConverter<TPtr, T *>(mcSharedToStar<TPtr, T *>);
        }
        auto pId = qRegisterMetaType<T *>();
        auto sId = qRegisterMetaType<TPtr>();
        McMetaTypeId::addQObjectPointerIds(pId, sId);
        McMetaTypeId::addSharedPointerId(sId, pId);

        const char *const cName = T::staticMetaObject.className();
        QByteArray typeName;
        typeName.append("QSharedPointer<").append(cName).append('>');
        qRegisterMetaType<TPtr>(typeName);

        typeName.clear();
        typeName.reserve(int(strlen(cName)) + 11);
        typeName.append(cName).append("ConstPtrRef");
        qRegisterMetaType<TPtr>(typeName);

        AddQObjectBuilderHelper<T, flag>::addQObjectBuilder(pId);
    }
};

template<typename T, bool flag>
void mcRegisterBeanFactory()
{
    Q_STATIC_ASSERT_X(!std::is_pointer<T>::value,
                      "mcRegisterBeanFactory's template type must not be a pointer type");
    McRegisterBeanFactory<T, flag>::registerBeanFactory();
}

template<typename T, bool flag, typename Enable = void>
struct McConverterRegister2
{
    static void registerConverter() { mcRegisterBeanFactory<T, flag>(); }
};

template<typename T, bool flag>
struct McConverterRegister2<T, flag, typename T::McPrivateTypeListHelper>
{
    static void registerConverter()
    {
        mcRegisterBeanFactory<T, flag>();
        using To = typename T::McPrivateTypeList;
        auto metaId = qMetaTypeId<QSharedPointer<T>>();
        McPrivate::McRegisterConverterHelper<T, To>::registerConverter(metaId);
    }
};

template<typename T, bool flag = std::is_default_constructible<T>::value>
struct McConverterRegister
{
    static void registerConverter() { McConverterRegister2<T, flag>::registerConverter(); }
};

template<typename T>
struct SharedTypeSelector
{
    typedef T BaseType; 
    typedef QSharedPointer<T> Type;
};
template<typename T>
struct SharedTypeSelector<T *>
{
    typedef T BaseType;
    typedef QSharedPointer<T> Type;
};
template<typename T>
struct SharedTypeSelector<QSharedPointer<T>>
{
    typedef T BaseType;
    typedef QSharedPointer<T> Type;
};

template<typename T>
struct PointerTypeSelector
{
    typedef T *Type;
};
template<typename T>
struct PointerTypeSelector<T *>
{
    typedef T *Type;
};

template<typename From, typename To>
struct McConverterList
{
    static To registerConverter(const From &from) { Q_UNUSED(from); }
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

template<typename T, typename Enable = void>
struct McMetaTypeIdHelper
{
    static int metaTypeId()
    {
        return qMetaTypeId<T>();
    }
};

template<typename T>
struct McMetaTypeIdHelper<
    T,
    typename std::enable_if<QtPrivate::IsSharedPointerToTypeDerivedFromQObject<T>::Value>::type>
{
    static int metaTypeId()
    {
        typedef typename T::Type ObjType;
        return qMetaTypeId<QSharedPointer<ObjType>>();
    }
};

template<typename T>
struct McMetaTypeIdHelper<
    T,
    typename std::enable_if<QtPrivate::IsPointerToTypeDerivedFromQObject<T>::Value>::type>
{
    static int metaTypeId()
    {
        typedef typename std::remove_pointer<T>::type ObjType;
        return qMetaTypeId<ObjType *>();
    }
};

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

template<typename T, typename Enable = void>
struct McContainerConverterRegisterHelper
{
    static void registerConverter(const char *typeName)
    {
        Q_UNUSED(typeName)
    }
};

template<typename T>
struct McContainerConverterRegisterHelper<
    T,
    typename std::enable_if<QtPrivate::IsSequentialContainer<T>::Value>::type>
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
struct McContainerConverterRegisterHelper<
    T,
    typename std::enable_if<QtPrivate::IsAssociativeContainer<T>::Value>::type>
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
