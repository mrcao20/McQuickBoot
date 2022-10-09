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
#include <QtCore/QSet>
#include <QtCore/QSharedPointer>
#include <QtCore/QVariant>
#include <QtCore/QWeakPointer>
#include <QtCore/QtGlobal>

#include "Destroyer/IMcDestroyer.h"
#include "McMacroGlobal.h"

#define MC_INTERFACES(...) \
 public: \
 using McPrivateTypeList = McPrivate::TypeList<__VA_ARGS__>; \
 using McPrivateTypeListHelper = void; \
\
 private:

#ifdef MC_USE_QT5
template<>
inline bool qMapLessThanKey(const QVariant &key1, const QVariant &key2)
{
    return qMapLessThanKey(key1.data(), key2.data());
}

# include <array>

namespace McPrivate {
struct TypeNormalizer
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
# ifdef Q_CC_MSVC
        /// On MSVC, keywords like class or struct are not separated with spaces in constexpr
        /// context
        if (msvcKw)
            return true;
# endif
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
# ifdef Q_CC_MSVC
            if (skipToken(begin, end, "__int64")) {
                numLong = 2;
                continue;
            }
# endif
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
        Q_UNUSED(begin);
        Q_UNUSED(end);
    }

public:
# if defined(Q_CC_CLANG) || defined(Q_CC_GNU)
    // this is much simpler than the full type normalization below
    // the reason is that the signature returned by Q_FUNC_INFO is already
    // normalized to the largest degree, and we need to do only small adjustments
    constexpr int normalizeTypeFromSignature(const char *begin, const char *end)
    {
        // bail out if there is an anonymous struct
        std::string_view name(begin, end - begin);
#  if defined(Q_CC_CLANG)
        if (name.find("anonymous ") != std::string_view::npos)
            return normalizeType(begin, end);
#  else
        if (name.find("unnamed ") != std::string_view::npos)
            return normalizeType(begin, end);
#  endif
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
# else
    // MSVC needs the full normalization, as it puts the const in a different
    // place than we expect
    constexpr int normalizeTypeFromSignature(const char *begin, const char *end) { return normalizeType(begin, end); }
# endif

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
struct is_std_pair : std::false_type
{
};

template<typename T1_, typename T2_>
struct is_std_pair<std::pair<T1_, T2_>> : std::true_type
{
    using T1 = T1_;
    using T2 = T2_;
};

template<typename T>
constexpr auto typenameHelper()
{
    if constexpr (is_std_pair<T>::value) {
        using T1 = typename is_std_pair<T>::T1;
        using T2 = typename is_std_pair<T>::T2;
        std::remove_const_t<decltype(typenameHelper<T1>())> t1Name = typenameHelper<T1>();
        std::remove_const_t<decltype(typenameHelper<T2>())> t2Name = typenameHelper<T2>();
        constexpr auto nonTypeDependentLen = sizeof("std::pair<,>");
        constexpr auto t1Len = t1Name.size() - 1;
        constexpr auto t2Len = t2Name.size() - 1;
        constexpr auto length = nonTypeDependentLen + t1Len + t2Len;
        std::array<char, length + 1> result{};
        constexpr auto prefix = "std::pair<";
        int currentLength = 0;
        for (; currentLength < int(sizeof("std::pair<") - 1); ++currentLength)
            result[currentLength] = prefix[currentLength];
        for (int i = 0; i < int(t1Len); ++currentLength, ++i)
            result[currentLength] = t1Name[i];
        result[currentLength++] = ',';
        for (int i = 0; i < int(t2Len); ++currentLength, ++i)
            result[currentLength] = t2Name[i];
        result[currentLength++] = '>';
        result[currentLength++] = '\0';
        return result;
    } else {
        constexpr auto prefix = sizeof(
# if defined(Q_CC_MSVC) && defined(Q_CC_CLANG)
                                    "auto __cdecl McPrivate::typenameHelper(void) [T = "
# elif defined(Q_CC_MSVC)
                                    "auto __cdecl McPrivate::typenameHelper<"
# elif defined(Q_CC_CLANG)
                                    "auto McPrivate::typenameHelper() [T = "
# elif defined(Q_CC_GHS)
                                    "auto McPrivate::typenameHelper<T>()[with T="
# else
                                    "constexpr auto McPrivate::typenameHelper() [with T = "
# endif
                                    )
                                - 1;
# if defined(Q_CC_MSVC) && !defined(Q_CC_CLANG)
        constexpr int suffix = sizeof(">(void)");
# else
        constexpr int suffix = sizeof("]");
# endif

# if (defined(Q_CC_GNU) && !defined(Q_CC_INTEL) && !defined(Q_CC_CLANG) && Q_CC_GNU < 804)
        auto func = Q_FUNC_INFO;
        const char *begin = func + prefix;
        const char *end = func + sizeof(Q_FUNC_INFO) - suffix;
        // This is an upper bound of the size since the normalized signature should always be smaller
        constexpr int len = sizeof(Q_FUNC_INFO) - suffix - prefix;
# else
        constexpr auto func = Q_FUNC_INFO;
        constexpr const char *begin = func + prefix;
        constexpr const char *end = func + sizeof(Q_FUNC_INFO) - suffix;
        constexpr int len = TypeNormalizer{nullptr}.normalizeTypeFromSignature(begin, end);
# endif
        std::array<char, len + 1> result{};
        TypeNormalizer{result.data()}.normalizeTypeFromSignature(begin, end);
        return result;
    }
}
} // namespace McPrivate
#else
inline bool operator<(const QVariant &left, const QVariant &right)
{
    auto res = QVariant::compare(left, right);
    return res == QPartialOrdering::Less;
}
#endif

class McMetaType;
namespace McPrivate {
#ifdef MC_USE_QT5
class MetaTypeInterface
{
public:
    //! 是否已经保存在容器中
    mutable QAtomicInteger<bool> isRegistered;
    //! 指针类型
    int pMetaType;
    //! 智能指针类型
    int sMetaType;
    //! 构造原始类型指针的函数指针
    using CreatePointerFn = void *(*)();
    CreatePointerFn createPointer;
    //! 构造QSharedPointer的函数指针
    using CreateSharedPointerFn = QVariant (*)(void *);
    CreateSharedPointerFn createSharedPointer;
    //! 继承的父类的元数据
    mutable QSet<McMetaType> *parents;
};

class ListMetaTypeInterface
{
public:
    //! 是否已经保存在容器中
    mutable QAtomicInteger<bool> isRegistered;
    //! 列表自身类型
    int metaType;
    //! 容器内存储的数据类型
    int valueMetaType;
};

class MapMetaTypeInterface
{
public:
    //! 是否已经保存在容器中
    mutable QAtomicInteger<bool> isRegistered;
    //! 映射自身类型
    int metaType;
    //! 容器内键的数据类型
    int keyMetaType;
    //! 容器内值的数据类型
    int valueMetaType;
};
#else
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
    mutable QSet<McMetaType> *parents;
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
#endif

template<typename S>
class MetaTypeForType
{
public:
#ifdef MC_USE_QT5
    static constexpr MetaTypeInterface::CreatePointerFn getCreatePointer() noexcept
    {
        if constexpr (std::is_default_constructible_v<S>) {
            return []() -> void * { return new S(); };
        } else {
            return nullptr;
        }
    }
#endif

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
            return [](void *copy) {
                S *inPtr = static_cast<S *>(copy);
                if (inPtr == nullptr) {
                    return QVariant();
                }
                if constexpr (std::is_base_of_v<IMcDestroyer, S>) {
                    return QVariant::fromValue(QSharedPointer<S>(inPtr, &IMcDestroyer::destroy));
                } else {
                    return QVariant::fromValue(QSharedPointer<S>(inPtr));
                }
            };
        }
    }
};

template<typename T, typename Enable = void>
struct MetaTypeInterfaceWrapper
{
#ifdef MC_USE_QT5
    static inline const MetaTypeInterface metaType = {
        /*.isRegistered=*/false,
        /*.pMetaType=*/qRegisterMetaType<T *>(),
        /*.sMetaType=*/qRegisterMetaType<QSharedPointer<T>>(typenameHelper<QSharedPointer<T>>().data()),
        /*.createPointer=*/MetaTypeForType<T>::getCreatePointer(),
        /*.createSharedPointer=*/MetaTypeForType<T>::getCreateSharedPointer(),
        /*.parents=*/nullptr,
    };
#else
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
#endif
};

template<typename T>
struct MetaTypeInterfaceWrapper<T, typename std::enable_if<QtPrivate::IsPointerToTypeDerivedFromQObject<T *>::Value>::type>
{
#ifdef MC_USE_QT5
    static inline const MetaTypeInterface metaType = {
        /*.isRegistered=*/false,
        /*.pMetaType=*/qRegisterMetaType<T *>(),
        /*.sMetaType=*/qRegisterMetaType<QSharedPointer<T>>(typenameHelper<QSharedPointer<T>>().data()),
        /*.createPointer=*/MetaTypeForType<T>::getCreatePointer(),
        /*.createSharedPointer=*/MetaTypeForType<T>::getCreateSharedPointer(),
        /*.parents=*/nullptr,
    };
#else
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
#endif
};

template<typename T>
struct ListMetaTypeInterfaceWrapper
{
#ifdef MC_USE_QT5
    static inline const ListMetaTypeInterface metaType = {
        /*.isRegistered=*/false,
        /*.metaType=*/qRegisterMetaType<T>(typenameHelper<T>().data()),
        /*.valueMetaType=*/qMetaTypeId<typename T::value_type>(),
    };
#else
    static inline constexpr const ListMetaTypeInterface metaType = {
        /*.isRegistered=*/false,
        /*.metaType=*/QMetaType::fromType<T>(),
        /*.valueMetaType=*/QMetaType::fromType<typename T::value_type>(),
    };
#endif
};

template<typename T>
struct MapMetaTypeInterfaceWrapper
{
#ifdef MC_USE_QT5
    static inline const MapMetaTypeInterface metaType = {
        /*.isRegistered=*/false,
        /*.metaType=*/qRegisterMetaType<T>(typenameHelper<T>().data()),
        /*.keyMetaType=*/qMetaTypeId<typename T::key_type>(),
        /*.valueMetaType=*/qMetaTypeId<typename T::mapped_type>(),
    };
#else
    static inline constexpr const MapMetaTypeInterface metaType = {
        /*.isRegistered=*/false,
        /*.metaType=*/QMetaType::fromType<T>(),
        /*.keyMetaType=*/QMetaType::fromType<typename T::key_type>(),
        /*.valueMetaType=*/QMetaType::fromType<typename T::mapped_type>(),
    };
#endif
};

template<typename T>
#ifdef MC_USE_QT5
const MetaTypeInterface *metaTypeInterfaceForType() noexcept
#else
constexpr const MetaTypeInterface *metaTypeInterfaceForType() noexcept
#endif
{
    using Ty = std::remove_cv_t<std::remove_reference_t<T>>;
    return &MetaTypeInterfaceWrapper<Ty>::metaType;
}

template<typename T>
#ifdef MC_USE_QT5
const ListMetaTypeInterface *listMetaTypeInterfaceForType() noexcept
#else
constexpr const ListMetaTypeInterface *listMetaTypeInterfaceForType() noexcept
#endif
{
    using Ty = std::remove_cv_t<std::remove_reference_t<T>>;
    return &ListMetaTypeInterfaceWrapper<Ty>::metaType;
}

template<typename T>
#ifdef MC_USE_QT5
const MapMetaTypeInterface *mapMetaTypeInterfaceForType() noexcept
#else
constexpr const MapMetaTypeInterface *mapMetaTypeInterfaceForType() noexcept
#endif
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

#ifdef MC_USE_QT6
    static McMetaType fromQMetaType(const QMetaType &type) noexcept;
    static McMetaType fromWQMetaType(const QMetaType &type) noexcept;
    static McMetaType fromTQMetaType(const QMetaType &type) noexcept;
    static McMetaType fromTypeName(const QByteArray &typeName) noexcept;
    static McMetaType fromWTypeName(const QByteArray &typeName) noexcept;
    static McMetaType fromTTypeName(const QByteArray &typeName) noexcept;
#endif

#ifdef MC_USE_QT5
    static McMetaType fromPQMetaType(int type) noexcept;
    static McMetaType fromSQMetaType(int type) noexcept;
    static McMetaType fromFuzzyQMetaType(int type) noexcept;
#else
    static McMetaType fromPQMetaType(const QMetaType &type) noexcept;
    static McMetaType fromSQMetaType(const QMetaType &type) noexcept;
    static McMetaType fromFuzzyQMetaType(const QMetaType &type) noexcept;
#endif
    static McMetaType fromPTypeName(const QByteArray &typeName) noexcept;
    static McMetaType fromSTypeName(const QByteArray &typeName) noexcept;
    static QVector<McMetaType> metaTypes() noexcept;

#ifdef MC_USE_QT5
    //! 指针类型
    constexpr int pMetaType() const noexcept
    {
        if (!isValid()) {
            return -1;
        }
        return d->pMetaType;
    }
    //! 智能指针类型
    constexpr int sMetaType() const noexcept
    {
        if (!isValid()) {
            return -1;
        }
        return d->sMetaType;
    }
#else
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
#endif

#ifdef MC_USE_QT5
    void *createPointer() const noexcept;
#endif
    QVariant createSharedPointer(void *copy = nullptr) const noexcept;

    void addParentMetaType(const McMetaType &type) const noexcept;
    QSet<McMetaType> parentMetaTypes() const noexcept;

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
#ifdef MC_USE_QT5
        return a.d->pMetaType == b.d->pMetaType && a.d->sMetaType == b.d->sMetaType;
#else
        return a.d->metaType == b.d->metaType && a.d->pMetaType == b.d->pMetaType && a.d->sMetaType == b.d->sMetaType
               && a.d->wMetaType == b.d->wMetaType && a.d->tMetaType == b.d->tMetaType;
#endif
    }
    friend bool operator!=(McMetaType a, McMetaType b)
    {
        return !(a == b);
    }
#ifdef MC_USE_QT5
    friend Q_DECL_CONST_FUNCTION uint qHash(McMetaType key, uint seed) noexcept
    {
        return qHash(key.sMetaType(), seed);
    }
#else
    friend Q_DECL_CONST_FUNCTION size_t qHash(McMetaType key, size_t seed) noexcept
    {
        return qHash(key.metaType().id(), seed);
    }
#endif

private:
    const McPrivate::MetaTypeInterface *d{nullptr};

    template<typename T>
    friend inline void mcRegisterMetaTypeSimple() noexcept;
    template<typename T>
    friend inline void mcRegisterMetaType() noexcept;
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

#ifdef MC_USE_QT5
    static McListMetaType fromQMetaType(int type) noexcept;
#else
    static McListMetaType fromQMetaType(const QMetaType &type) noexcept;
#endif
    static QVector<McListMetaType> metaTypes() noexcept;

#ifdef MC_USE_QT5
    ///! 列表自身类型
    constexpr int metaType() const noexcept
    {
        if (!isValid()) {
            return -1;
        }
        return d->metaType;
    }
    //! 容器内存储的数据类型
    constexpr int valueMetaType() const noexcept
    {
        if (!isValid()) {
            return -1;
        }
        return d->valueMetaType;
    }
#else
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
#endif

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

    template<typename T>
    friend inline void mcRegisterContainer() noexcept;
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

#ifdef MC_USE_QT5
    static McMapMetaType fromQMetaType(int type) noexcept;
#else
    static McMapMetaType fromQMetaType(const QMetaType &type) noexcept;
#endif
    static QVector<McMapMetaType> metaTypes() noexcept;

#ifdef MC_USE_QT5
    //! 映射自身类型
    constexpr int metaType() const noexcept
    {
        if (!isValid()) {
            return -1;
        }
        return d->metaType;
    }
    //! 容器内键的数据类型
    constexpr int keyMetaType() const noexcept
    {
        if (!isValid()) {
            return -1;
        }
        return d->keyMetaType;
    }
    //! 容器内值的数据类型
    constexpr int valueMetaType() const noexcept
    {
        if (!isValid()) {
            return -1;
        }
        return d->valueMetaType;
    }
#else
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
#endif

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

    template<typename T>
    friend inline void mcRegisterContainer() noexcept;
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
#ifdef MC_USE_QT6
        if constexpr (!bool(QtPrivate::IsPointerToTypeDerivedFromQObject<From *>::Value)
                      || !bool(QtPrivate::IsPointerToTypeDerivedFromQObject<To *>::Value)) {
#endif
            if (!QMetaType::hasRegisteredConverterFunction<From *, To *>()) {
                QMetaType::registerConverter<From *, To *>();
            }
#ifdef MC_USE_QT6
        }
#endif
        if (!QMetaType::hasRegisteredConverterFunction<FromPtr, To *>()) {
            QMetaType::registerConverter<FromPtr, To *>([](const FromPtr &from) { return from.operator->(); });
        }
#ifdef MC_USE_QT5
        McMetaType srcMetaType = McMetaType::fromType<From>();
        McMetaType dstMetaType = McMetaType::fromType<To>();
#else
        constexpr McMetaType srcMetaType = McMetaType::fromType<From>();
        constexpr McMetaType dstMetaType = McMetaType::fromType<To>();
#endif
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
#ifdef MC_USE_QT5
    McMetaType metaType = McMetaType::fromType<T>();
#else
    constexpr McMetaType metaType = McMetaType::fromType<T>();
#endif
    if (metaType.d->isRegistered.loadRelaxed()) {
        return;
    }
    McMetaType::registerMetaType(metaType);
}

template<typename T>
inline void mcRegisterMetaType() noexcept
{
    static_assert(!std::is_pointer<T>::value, "mcRegisterMetaType's template type must not be a pointer type");
#ifdef MC_USE_QT5
    McMetaType metaType = McMetaType::fromType<T>();
#else
    constexpr McMetaType metaType = McMetaType::fromType<T>();
#endif
    if (metaType.d->isRegistered.loadRelaxed()) {
        return;
    }
    mcRegisterMetaTypeSimple<T>();
    McPrivate::MetaTypeRegister<T>::registerMetaType();
}

template<typename T>
inline void mcRegisterContainer() noexcept
{
    if constexpr (bool(QtPrivate::IsSequentialContainer<T>::Value)) {
#ifdef MC_USE_QT5
        McListMetaType customMetaType = McListMetaType::fromType<T>();
#else
        constexpr McListMetaType customMetaType = McListMetaType::fromType<T>();
#endif
        if (customMetaType.d->isRegistered.loadRelaxed()) {
            return;
        }
#ifdef MC_USE_QT5
        int metaType = customMetaType.metaType();
#else
        constexpr QMetaType metaType = customMetaType.metaType();
#endif
        using ValueType = typename T::value_type;
        mcRegisterContainer<ValueType>();
        McListMetaType::registerMetaType(customMetaType);
#ifdef MC_USE_QT5
        int listMetaType = qMetaTypeId<QVariantList>();
#else
        constexpr QMetaType listMetaType = QMetaType::fromType<QVariantList>();
#endif
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
#ifdef MC_USE_QT5
        McMapMetaType customMetaType = McMapMetaType::fromType<T>();
#else
        constexpr McMapMetaType customMetaType = McMapMetaType::fromType<T>();
#endif
        if (customMetaType.d->isRegistered.loadRelaxed()) {
            return;
        }
#ifdef MC_USE_QT5
        int metaType = customMetaType.metaType();
#else
        constexpr QMetaType metaType = customMetaType.metaType();
#endif
        using KeyType = typename T::key_type;
        using ValueType = typename T::mapped_type;
        mcRegisterContainer<KeyType>();
        mcRegisterContainer<ValueType>();
        McMapMetaType::registerMetaType(customMetaType);
#ifdef MC_USE_QT5
        int mapMetaType = qMetaTypeId<QMap<QVariant, QVariant>>();
#else
        constexpr QMetaType mapMetaType = QMetaType::fromType<QMap<QVariant, QVariant>>();
#endif
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
