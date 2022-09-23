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

#include <array>

#include <QCoreApplication>
#include <QMetaType>
#include <QVariant>

#include <QHash>
#include <QSharedPointer>

#include "McMacroGlobal.h"

#define MC_TYPELIST(...) \
 public: \
 using McPrivateTypeList = McPrivate::McTypeList<__VA_ARGS__>; \
 using McPrivateTypeListHelper = void; \
\
 private:

#define MC_INTERFACES(...) MC_TYPELIST(__VA_ARGS__)

template<>
inline bool qMapLessThanKey(const QVariant &key1, const QVariant &key2)
{
    return qMapLessThanKey(key1.data(), key2.data());
}

struct McPointerMetaId
{
    int qobjectPointerId{-1};
    int sharedPointerId{-1};
};
MC_DECL_POINTER(McPointerMetaId)

struct McGadgetMetaId
{
    int gadgetId{-1};
    int pointerId{-1};
    int sharedId{-1};
};
MC_DECL_POINTER(McGadgetMetaId)

struct McSequentialMetaId
{
    int id{-1};
    int valueId{-1};
};
MC_DECL_POINTER(McSequentialMetaId)

struct McAssociativeMetaId
{
    int id{-1};
    int keyId{-1};
    int valueId{-1};
};
MC_DECL_POINTER(McAssociativeMetaId)

typedef QHash<int, McPointerMetaIdPtr> McPointerType;
typedef QHash<int, McGadgetMetaIdPtr> McGadgetType;
typedef QHash<int, McSequentialMetaIdPtr> McSequentialType;
typedef QHash<int, McAssociativeMetaIdPtr> McAssociativeType;
typedef QHash<int, QList<int>> McMetaIdMapType;
//! beanName应该用QByteArray保存
//! <sharedId, 某个共享指针类型对应的beanName>
typedef QHash<int, QStringList> McBeanNameMapType;

class MC_CORE_EXPORT McMetaTypeId
{
public:
    static McPointerType qobjectPointerIds() noexcept;
    static void addQObjectPointerIds(int id, int sharedId) noexcept;
    static int getSharedForPointer(int id) noexcept;

    static McPointerType sharedPointerIds() noexcept;
    static void addSharedPointerId(int id, int qobjectId) noexcept;
    static int getPointerForShared(int id) noexcept;

    static McGadgetType gadgetIds() noexcept;
    static void addGadget(int gId, int pId, int sId) noexcept;

    static McSequentialType sequentialIds() noexcept;
    static void addSequentialId(int id, int valueId) noexcept;
    static int getSequentialValueId(int id) noexcept;

    static McAssociativeType associativeIds() noexcept;
    static void addAssociativeId(int id, int keyId, int valueId) noexcept;

    static McMetaIdMapType metaIdMapType() noexcept;
    static void addMetaIdMap(int mainMetaId, int dstMetaId) noexcept;
    static QList<int> getDstMetaIds(int id) noexcept;

    static McBeanNameMapType beanNameMapType() noexcept;
    static void addBeanNameMap(int id, const QString &beanName) noexcept;
    static void addBeanNameMap(const QList<int> &ids, const QString &beanName) noexcept;
    static QStringList getBeanNamesForId(int id) noexcept;
};

namespace McPrivate {
template<typename...>
struct McTypeList;

template<typename T, typename... U>
struct McTypeList<T, U...>
{
    using Head = T;
    using Tails = McTypeList<U...>;
};

// 针对空list的特化
template<>
struct McTypeList<>
{
};

MC_FORWARD_DECL_CLASS(IQObjectBuilder);

class MC_CORE_EXPORT IQObjectBuilder
{
public:
    virtual ~IQObjectBuilder() = default;

    virtual QObject *create() noexcept = 0;

    static void addQObjectBuilder(int id, const IQObjectBuilderPtr &builder) noexcept;
    template<typename T>
    static void addQObjectBuilder() noexcept;
    static IQObjectBuilderPtr getQObjectBuilder(int id) noexcept;
};

template<typename T>
class QObjectBuilder : public IQObjectBuilder
{
public:
    QObject *create() noexcept override { return new T(); }
};

template<typename T>
using QObjectBuilderPtr = QSharedPointer<QObjectBuilder<T>>;

template<typename T>
void IQObjectBuilder::addQObjectBuilder() noexcept
{
    auto id = qMetaTypeId<std::remove_pointer_t<T> *>();
    auto builder = QObjectBuilderPtr<std::remove_pointer_t<T>>::create();
    IQObjectBuilder::addQObjectBuilder(id, builder);
}

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
    static void registerConverter(int metaId) { McRegisterConverterHelper2<From, To>::registerConverter(metaId); }
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
struct McRegisterBeanFactory<T, flag, typename std::enable_if<QtPrivate::IsGadgetHelper<T>::IsRealGadget>::type>
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
struct McRegisterBeanFactory<T, flag,
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
            QMetaType::registerConverter<QObjectPtr, TPtr>(mcConverterQSharedPointerObject<QObjectPtr, TPtr>);
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
    Q_STATIC_ASSERT_X(!std::is_pointer<T>::value, "mcRegisterBeanFactory's template type must not be a pointer type");
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
    static To registerConverter(const From &from) { Q_UNUSED(from); }
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
    static int metaTypeId() { return qMetaTypeId<T>(); }
};

template<typename T>
struct McMetaTypeIdHelper<T, typename std::enable_if<QtPrivate::IsSharedPointerToTypeDerivedFromQObject<T>::Value>::type>
{
    static int metaTypeId()
    {
        typedef typename T::Type ObjType;
        return qMetaTypeId<QSharedPointer<ObjType>>();
    }
};

template<typename T>
struct McMetaTypeIdHelper<T, typename std::enable_if<QtPrivate::IsPointerToTypeDerivedFromQObject<T>::Value>::type>
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
        if (var.canConvert<To>()) {
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
        if (var.canConvert<To>()) {
            return;
        }
        QMetaType::registerConverter<From, To>(McPrivate::McConverterMap<From, To>::registerConverter);
    }
};

template<typename T, typename Enable = void>
struct McContainerConverterRegisterHelper
{
    static void registerConverter(const char *typeName) { Q_UNUSED(typeName) }
};

template<typename T>
struct McContainerConverterRegisterHelper<T, typename std::enable_if<QtPrivate::IsSequentialContainer<T>::Value>::type>
{
    static void registerConverter(const char *typeName)
    {
        auto id = QMetaType::type(typeName);
        if (id == QMetaType::UnknownType) {
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
struct McContainerConverterRegisterHelper<T, typename std::enable_if<QtPrivate::IsAssociativeContainer<T>::Value>::type>
{
    static void registerConverter(const char *typeName)
    {
        auto id = QMetaType::type(typeName);
        if (id == QMetaType::UnknownType) {
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

struct QTypeNormalizer
{
    char *output;
    int len = 0;
    char last = 0;

private:
    static constexpr bool is_ident_char(char s)
    {
        return ((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z') || (s >= '0' && s <= '9') || s == '_');
    }
    static constexpr bool is_space(char s) { return (s == ' ' || s == '\t' || s == '\n'); }
    static constexpr bool is_number(char s) { return s >= '0' && s <= '9'; }
    static constexpr bool starts_with_token(const char *b, const char *e, const char *token, bool msvcKw = false)
    {
        while (b != e && *token && *b == *token) {
            b++;
            token++;
        }
        if (*token)
            return false;
#ifdef Q_CC_MSVC
        /// On MSVC, keywords like class or struct are not separated with spaces in constexpr
        /// context
        if (msvcKw)
            return true;
#endif
        Q_UNUSED(msvcKw);
        return b == e || !is_ident_char(*b);
    }
    static constexpr bool skipToken(const char *&x, const char *e, const char *token, bool msvcKw = false)
    {
        if (!starts_with_token(x, e, token, msvcKw))
            return false;
        while (*token++)
            x++;
        while (x != e && is_space(*x))
            x++;
        return true;
    }
    static constexpr const char *skipString(const char *x, const char *e)
    {
        char delim = *x;
        x++;
        while (x != e && *x != delim) {
            if (*x == '\\') {
                x++;
                if (x == e)
                    return e;
            }
            x++;
        }
        if (x != e)
            x++;
        return x;
    };
    static constexpr const char *skipTemplate(const char *x, const char *e, bool stopAtComa = false)
    {
        int scopeDepth = 0;
        int templateDepth = 0;
        while (x != e) {
            switch (*x) {
            case '<':
                if (!scopeDepth)
                    templateDepth++;
                break;
            case ',':
                if (stopAtComa && !scopeDepth && !templateDepth)
                    return x;
                break;
            case '>':
                if (!scopeDepth)
                    if (--templateDepth < 0)
                        return x;
                break;
            case '(':
            case '[':
            case '{':
                scopeDepth++;
                break;
            case '}':
            case ']':
            case ')':
                scopeDepth--;
                break;
            case '\'':
                if (is_number(x[-1]))
                    break;
                Q_FALLTHROUGH();
            case '\"':
                x = skipString(x, e);
                continue;
            }
            x++;
        }
        return x;
    };

    constexpr void append(char x)
    {
        last = x;
        len++;
        if (output)
            *output++ = x;
    }

    constexpr void replaceLast(char x)
    {
        last = x;
        if (output)
            *(output - 1) = x;
    }

    constexpr void appendStr(const char *x)
    {
        while (*x)
            append(*x++);
    };

    constexpr void normalizeIntegerTypes(const char *&begin, const char *end)
    {
        int numLong = 0;
        int numSigned = 0;
        int numUnsigned = 0;
        int numInt = 0;
        int numShort = 0;
        int numChar = 0;
        while (begin < end) {
            if (skipToken(begin, end, "long")) {
                numLong++;
                continue;
            }
            if (skipToken(begin, end, "int")) {
                numInt++;
                continue;
            }
            if (skipToken(begin, end, "short")) {
                numShort++;
                continue;
            }
            if (skipToken(begin, end, "unsigned")) {
                numUnsigned++;
                continue;
            }
            if (skipToken(begin, end, "signed")) {
                numSigned++;
                continue;
            }
            if (skipToken(begin, end, "char")) {
                numChar++;
                continue;
            }
#ifdef Q_CC_MSVC
            if (skipToken(begin, end, "__int64")) {
                numLong = 2;
                continue;
            }
#endif
            break;
        }
        if (numLong == 2)
            append('q'); // q(u)longlong
        if (numSigned && numChar)
            appendStr("signed ");
        else if (numUnsigned)
            appendStr("u");
        if (numChar)
            appendStr("char");
        else if (numShort)
            appendStr("short");
        else if (numLong == 1)
            appendStr("long");
        else if (numLong == 2)
            appendStr("longlong");
        else if (numUnsigned || numSigned || numInt)
            appendStr("int");
    }

    constexpr void skipStructClassOrEnum(const char *&begin, const char *end)
    {
        // discard 'struct', 'class', and 'enum'; they are optional
        // and we don't want them in the normalized signature
        skipToken(begin, end, "struct", true) || skipToken(begin, end, "class", true)
            || skipToken(begin, end, "enum", true);
    }

    constexpr void skipQtNamespace(const char *&begin, const char *end)
    {
#ifdef QT_NAMESPACE
        const char *nsbeg = begin;
        if (skipToken(nsbeg, end, QT_STRINGIFY(QT_NAMESPACE)) && nsbeg + 2 < end && nsbeg[0] == ':' && nsbeg[1] == ':') {
            begin = nsbeg + 2;
            while (begin != end && is_space(*begin))
                begin++;
        }
#else
        Q_UNUSED(begin);
        Q_UNUSED(end);
#endif
    }

public:
#if defined(Q_CC_CLANG) || defined(Q_CC_GNU)
    // this is much simpler than the full type normalization below
    // the reason is that the signature returned by Q_FUNC_INFO is already
    // normalized to the largest degree, and we need to do only small adjustments
    constexpr int normalizeTypeFromSignature(const char *begin, const char *end)
    {
        // bail out if there is an anonymous struct
        std::string_view name(begin, end - begin);
# if defined(Q_CC_CLANG)
        if (name.find("anonymous ") != std::string_view::npos)
            return normalizeType(begin, end);
# else
        if (name.find("unnamed ") != std::string_view::npos)
            return normalizeType(begin, end);
# endif
        while (begin < end) {
            if (*begin == ' ') {
                if (last == ',' || last == '>' || last == '<' || last == '*' || last == '&') {
                    ++begin;
                    continue;
                }
            }
            if (last == ' ') {
                if (*begin == '*' || *begin == '&' || *begin == '(') {
                    replaceLast(*begin);
                    ++begin;
                    continue;
                }
            }
            if (!is_ident_char(last)) {
                skipStructClassOrEnum(begin, end);
                if (begin == end)
                    break;

                skipQtNamespace(begin, end);
                if (begin == end)
                    break;

                normalizeIntegerTypes(begin, end);
                if (begin == end)
                    break;
            }
            append(*begin);
            ++begin;
        }
        return len;
    }
#else
    // MSVC needs the full normalization, as it puts the const in a different
    // place than we expect
    constexpr int normalizeTypeFromSignature(const char *begin, const char *end)
    {
        return normalizeType(begin, end);
    }
#endif

    constexpr int normalizeType(const char *begin, const char *end, bool adjustConst = true)
    {
        // Trim spaces
        while (begin != end && is_space(*begin))
            begin++;
        while (begin != end && is_space(*(end - 1)))
            end--;

        // Convert 'char const *' into 'const char *'. Start at index 1,
        // not 0, because 'const char *' is already OK.
        const char *cst = begin + 1;
        if (*begin == '\'' || *begin == '"')
            cst = skipString(begin, end);
        bool seenStar = false;
        bool hasMiddleConst = false;
        while (cst < end) {
            if (*cst == '\"' || (*cst == '\'' && !is_number(cst[-1]))) {
                cst = skipString(cst, end);
                if (cst == end)
                    break;
            }

            // We mustn't convert 'char * const *' into 'const char **'
            // and we must beware of 'Bar<const Bla>'.
            if (*cst == '&' || *cst == '*' || *cst == '[') {
                seenStar = *cst != '&' || cst != (end - 1);
                break;
            }
            if (*cst == '<') {
                cst = skipTemplate(cst + 1, end);
                if (cst == end)
                    break;
            }
            cst++;
            const char *skipedCst = cst;
            if (!is_ident_char(*(cst - 1)) && skipToken(skipedCst, end, "const")) {
                const char *testEnd = end;
                while (skipedCst < testEnd--) {
                    if (*testEnd == '*' || *testEnd == '[' || (*testEnd == '&' && testEnd != (end - 1))) {
                        seenStar = true;
                        break;
                    }
                    if (*testEnd == '>')
                        break;
                }
                if (adjustConst && !seenStar) {
                    if (*(end - 1) == '&')
                        end--;
                } else {
                    appendStr("const ");
                }
                normalizeType(begin, cst, false);
                begin = skipedCst;
                hasMiddleConst = true;
                break;
            }
        }
        if (skipToken(begin, end, "const")) {
            if (adjustConst && !seenStar) {
                if (*(end - 1) == '&')
                    end--;
            } else {
                appendStr("const ");
            }
        }
        if (seenStar && adjustConst) {
            const char *e = end;
            if (*(end - 1) == '&' && *(end - 2) != '&')
                e--;
            while (begin != e && is_space(*(e - 1)))
                e--;
            const char *token = "tsnoc"; // 'const' reverse, to check if it ends with const
            while (*token && begin != e && *(--e) == *token++)
                ;
            if (!*token && begin != e && !is_ident_char(*(e - 1))) {
                while (begin != e && is_space(*(e - 1)))
                    e--;
                end = e;
            }
        }

        skipStructClassOrEnum(begin, end);
        skipQtNamespace(begin, end);

        if (skipToken(begin, end, "QVector")) {
            // Replace QVector by QList
            appendStr("QList");
        }

        if (skipToken(begin, end, "QPair")) {
            // replace QPair by std::pair
            appendStr("std::pair");
        }

        if (!hasMiddleConst)
            // Normalize the integer types
            normalizeIntegerTypes(begin, end);

        bool spaceSkiped = true;
        while (begin != end) {
            char c = *begin++;
            if (is_space(c)) {
                spaceSkiped = true;
            } else if ((c == '\'' && !is_number(last)) || c == '\"') {
                begin--;
                auto x = skipString(begin, end);
                while (begin < x)
                    append(*begin++);
            } else {
                if (spaceSkiped && is_ident_char(last) && is_ident_char(c))
                    append(' ');
                append(c);
                spaceSkiped = false;
                if (c == '<') {
                    do {
                        // template recursion
                        const char *tpl = skipTemplate(begin, end, true);
                        normalizeType(begin, tpl, false);
                        if (tpl == end)
                            return len;
                        append(*tpl);
                        begin = tpl;
                    } while (*begin++ == ',');
                }
            }
        }
        return len;
    }
};

template<typename T>
constexpr auto typenameHelper()
{
    constexpr auto prefix = sizeof(
#ifdef QT_NAMESPACE
                                QT_STRINGIFY(QT_NAMESPACE) "::"
#endif
#if defined(Q_CC_MSVC) && defined(Q_CC_CLANG)
                                                           "auto __cdecl QtPrivate::typenameHelper(void) [T = "
#elif defined(Q_CC_MSVC)
                                "auto __cdecl QtPrivate::typenameHelper<"
#elif defined(Q_CC_CLANG)
                                "auto QtPrivate::typenameHelper() [T = "
#elif defined(Q_CC_GHS)
                                "auto QtPrivate::typenameHelper<T>()[with T="
#else
                                "constexpr auto QtPrivate::typenameHelper() [with T = "
#endif
                                )
                            - 1;
#if defined(Q_CC_MSVC) && !defined(Q_CC_CLANG)
    constexpr int suffix = sizeof(">(void)");
#else
    constexpr int suffix = sizeof("]");
#endif

#if (defined(Q_CC_GNU) && !defined(Q_CC_INTEL) && !defined(Q_CC_CLANG) && Q_CC_GNU < 804)
    auto func = Q_FUNC_INFO;
    const char *begin = func + prefix;
    const char *end = func + sizeof(Q_FUNC_INFO) - suffix;
    // This is an upper bound of the size since the normalized signature should always be smaller
    constexpr int len = sizeof(Q_FUNC_INFO) - suffix - prefix;
#else
    constexpr auto func = Q_FUNC_INFO;
    constexpr const char *begin = func + prefix;
    constexpr const char *end = func + sizeof(Q_FUNC_INFO) - suffix;
    constexpr int len = QTypeNormalizer{nullptr}.normalizeTypeFromSignature(begin, end);
#endif
    std::array<char, len + 1> result{};
    QTypeNormalizer{result.data()}.normalizeTypeFromSignature(begin, end);
    return result;
}
} // namespace McPrivate

template<typename T>
inline void mcRegisterMetaTypeSimple() noexcept
{
    McPrivate::McConverterRegister<T>::registerConverter();
}

template<typename T>
inline void mcRegisterMetaType() noexcept
{
    McPrivate::McConverterRegister<T>::registerConverter();
}

template<typename T>
inline void mcRegisterContainer() noexcept
{
    McPrivate::McContainerConverterRegisterHelper<T>::registerConverter(McPrivate::typenameHelper<T>());
}
