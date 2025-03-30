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
#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/qglobal.h>

#if __has_include("McConfig.h")
# include "McConfig.h"

# define MC_QUICKBOOT_VERSION \
  QT_VERSION_CHECK(MC_QUICKBOOT_VERSION_MAJOR, MC_QUICKBOOT_VERSION_MINOR, MC_QUICKBOOT_VERSION_PATCH)
#else
# define MC_QUICKBOOT_VERSION QT_VERSION_CHECK(0, 0, 0)
#endif

#if defined(MC_BUILD_STATIC) && !defined(MC_EXPORT_DISABLE)
# define MC_EXPORT_DISABLE
#endif

#ifndef MC_EXPORT_DISABLE
# if defined(MC_CORE_LIBRARY)
#  define MC_CORE_EXPORT Q_DECL_EXPORT
# else
#  define MC_CORE_EXPORT Q_DECL_IMPORT
# endif
#else
# define MC_CORE_EXPORT
#endif

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
# define MC_USE_QT5
#else
# define MC_USE_QT6
#endif

// clang-format off
#define MC_STRINGIFY(x) #x

#define MC_SAFETY_DELETE(p) \
    { \
        if (p != nullptr) { \
            delete p; \
            p = nullptr; \
        } \
    }

#define MC_SAFETY_DELETE2(p) \
    { \
        if (p != nullptr) { \
            delete[] p; \
            p = nullptr; \
        } \
    }

#define MC_DECL_POINTER(Class) \
    using Class##Ptr = QSharedPointer<Class>; \
    using Class##WPtr = QWeakPointer<Class>;
#define MC_DECL_U_POINTER(Class, ...) \
    using Class##UPtr = QScopedPointer<Class, __VA_ARGS__>;
#define MC_DECL_STL_POINTER(Class) \
    using Class##Ptr = std::shared_ptr<Class>; \
    using Class##WPtr = std::weak_ptr<Class>;
#define MC_DECL_STL_U_POINTER(Class, ...) \
    using Class##UPtr = std::unique_ptr<Class, __VA_ARGS__>;

#define MC_DECL_POINTER_NS(Class, NS) \
    namespace NS { \
        MC_DECL_POINTER(Class) \
    }
#define MC_DECL_U_POINTER_NS(Class, NS, ...) \
    namespace NS { \
        MC_DECL_U_POINTER(Class, __VA_ARGS__) \
    }
#define MC_DECL_STL_POINTER_NS(Class, NS) \
    namespace NS { \
        MC_DECL_STL_POINTER(Class) \
    }
#define MC_DECL_STL_U_POINTER_NS(Class, NS, ...) \
    namespace NS { \
        MC_DECL_STL_U_POINTER(Class, __VA_ARGS__) \
    }

#define MC_FORWARD_DECL_CLASS(Class) \
    class Class; \
    MC_DECL_POINTER(Class)
#define MC_FORWARD_DECL_U_CLASS(Class, ...) \
    class Class; \
    MC_DECL_U_POINTER(Class, __VA_ARGS__)
#define MC_FORWARD_DECL_STL_CLASS(Class) \
    class Class; \
    MC_DECL_STL_POINTER(Class)
#define MC_FORWARD_DECL_STL_U_CLASS(Class, ...) \
    class Class; \
    MC_DECL_STL_U_POINTER(Class, __VA_ARGS__)

#define MC_FORWARD_DECL_CLASS_NS(Class, NS) \
    namespace NS { \
        MC_FORWARD_DECL_CLASS(Class) \
    }
#define MC_FORWARD_DECL_U_CLASS_NS(Class, NS, ...) \
    namespace NS { \
        MC_FORWARD_DECL_U_CLASS(Class, __VA_ARGS__) \
    }
#define MC_FORWARD_DECL_STL_CLASS_NS(Class, NS) \
    namespace NS { \
        MC_FORWARD_DECL_STL_CLASS(Class) \
    }
#define MC_FORWARD_DECL_STL_U_CLASS_NS(Class, NS, ...) \
    namespace NS { \
        MC_FORWARD_DECL_STL_U_CLASS(Class, __VA_ARGS__) \
    }

#define MC_FORWARD_DECL_STRUCT(Class) \
    struct Class; \
    MC_DECL_POINTER(Class)
#define MC_FORWARD_DECL_U_STRUCT(Class, ...) \
    struct Class; \
    MC_DECL_U_POINTER(Class, __VA_ARGS__)
#define MC_FORWARD_DECL_STL_STRUCT(Class) \
    struct Class; \
    MC_DECL_STL_POINTER(Class)
#define MC_FORWARD_DECL_STL_U_STRUCT(Class, ...) \
    struct Class; \
    MC_DECL_STL_U_POINTER(Class, __VA_ARGS__)

#define MC_FORWARD_DECL_STRUCT_NS(Class, NS) \
    namespace NS { \
        MC_FORWARD_DECL_STRUCT(Class) \
    }
#define MC_FORWARD_DECL_U_STRUCT_NS(Class, NS, ...) \
    namespace NS { \
        MC_FORWARD_DECL_U_STRUCT(Class, __VA_ARGS__) \
    }
#define MC_FORWARD_DECL_STL_STRUCT_NS(Class, NS) \
    namespace NS { \
        MC_FORWARD_DECL_STL_STRUCT(Class) \
    }
#define MC_FORWARD_DECL_STL_U_STRUCT_NS(Class, NS, ...) \
    namespace NS { \
        MC_FORWARD_DECL_STL_U_STRUCT(Class, __VA_ARGS__) \
    }

#define MC_PRIVATE_DATA_NAME(Class) __Mc_##Class##Data__

#define MC_DECL_PRIVATE_DATA(Class) \
    struct MC_PRIVATE_DATA_NAME(Class) \
    {
#define MC_DECL_PRIVATE_DATA_END \
    };

#define MC_DECL_PRIVATE_DATA2(Class) struct MC_PRIVATE_DATA_NAME(Class)

#define MC_DECL_SHARED_PRIVATE_DATA(Class) \
    struct MC_PRIVATE_DATA_NAME(Class) \
        : public QSharedData \
    {
#define MC_DECL_SHARED_PRIVATE_DATA_END \
    };

#define MC_DECL_SHARED_PRIVATE_DATA2(Class) \
    struct MC_PRIVATE_DATA_NAME(Class) \
        : public QSharedData

#define MC_PRIVATE_DATA_DESTRUCTOR(Class) \
    ~MC_PRIVATE_DATA_NAME(Class)() \
    {
//
#define MC_PRIVATE_DATA_DESTRUCTOR_END }

#define MC_PRIVATE_DATA_DESTRUCTOR2(Class) ~MC_PRIVATE_DATA_NAME(Class)()

#define MC_PRIVATE_DATA_TYPE(Class) QScopedPointer<MC_PRIVATE_DATA_NAME(Class)>
#define MC_COPYABLE_PRIVATE_DATA_TYPE(Class) std::unique_ptr<MC_PRIVATE_DATA_NAME(Class)>
#define MC_SHARED_PRIVATE_DATA_TYPE(Class) QExplicitlySharedDataPointer<MC_PRIVATE_DATA_NAME(Class)>

#define MC_DECL_PRIVATE(Class) \
    MC_PRIVATE_DATA_TYPE(Class) d; \
    friend struct MC_PRIVATE_DATA_NAME(Class);

#define MC_DECL_COPYABLE_PRIVATE(Class) \
    MC_COPYABLE_PRIVATE_DATA_TYPE(Class) d; \
    friend struct MC_PRIVATE_DATA_NAME(Class);

#define MC_DECL_SHARED_PRIVATE(Class) \
    MC_SHARED_PRIVATE_DATA_TYPE(Class) d; \
    friend struct MC_PRIVATE_DATA_NAME(Class);

#define MC_NEW_PRIVATE_DATA(Class) d.reset(new MC_PRIVATE_DATA_NAME(Class)());
#define MC_NEW_SHARED_PRIVATE_DATA(Class) d = new MC_PRIVATE_DATA_NAME(Class)();

#define MC_FORWARD_DECL_PRIVATE_DATA(Class) struct MC_PRIVATE_DATA_NAME(Class);

#define MC_PADDING_CLANG(size) char ___clang_padding___[size];

#define MC_DECL_INIT(Class) static const int Class##_Static_Init;

#define MC_DECL_SUPER(SUPER) \
    private: \
    typedef SUPER super;

#define MC_INIT(Class, ...) const int Class::Class##_Static_Init = []() -> int { \
    using ClassType = Class; \
    { \
        ClassType *_ = nullptr; \
        Q_UNUSED(_); \
    } \
    constexpr auto __prePriority__ = McPrivate::extractRoutinePriority(__VA_ARGS__); \
    Mc::addPreRoutine(__prePriority__, [](){
#define MC_AUTO_INIT(Class, ...) \
    MC_INIT(Class, __VA_ARGS__) \
    mcRegisterMetaType<Class>();
#define MC_DESTROY(...) \
    }); \
    constexpr auto __postPriority__ = McPrivate::extractRoutinePriority(__VA_ARGS__); \
    Mc::addPostRoutine(__postPriority__, [](){
#define MC_INIT_END \
    }); \
    return 0; \
}();

#define MC_STATIC(...) static const int __Static_Code_Block__ = []() -> int { \
    constexpr auto __prePriority__ = McPrivate::extractRoutinePriority(__VA_ARGS__); \
    Mc::addPreRoutine(__prePriority__, [](){
#define MC_STATIC_END \
    }); \
    return 0; \
}();

#define MC_INIT2_HELPER(Class, EXTRA, ...) \
    static void Class##__Static_Code_Block_Constructor_Func__() noexcept; \
    const int Class::Class##_Static_Init = []() -> int { \
        using ClassType = Class; \
        { \
            ClassType *_ = nullptr; \
            Q_UNUSED(_); \
        } \
        constexpr auto __prePriority__ = McPrivate::extractRoutinePriority(__VA_ARGS__); \
        Mc::addPreRoutine(__prePriority__, []() { EXTRA Class##__Static_Code_Block_Constructor_Func__(); }); \
        return 0; \
    }(); \
    static void Class##__Static_Code_Block_Constructor_Func__() noexcept
#define MC_INIT2(Class, ...) MC_INIT2_HELPER(Class, Q_UNUSED((int *)nullptr);, __VA_ARGS__)
#define MC_AUTO_INIT2(Class, ...) MC_INIT2_HELPER(Class, mcRegisterMetaType<Class>();, __VA_ARGS__)

#define MC_STATIC2(...) \
    static void __Static_Code_Block_Constructor_Func__() noexcept; \
    static const int __Static_Code_Block_Constructor__ = []() -> int { \
        constexpr auto __prePriority__ = McPrivate::extractRoutinePriority(__VA_ARGS__); \
        Mc::addPreRoutine(__prePriority__, []() { __Static_Code_Block_Constructor_Func__(); }); \
        return 0; \
    }(); \
    static void __Static_Code_Block_Constructor_Func__() noexcept

#define MC_DESTROY2(...) \
    static void __Static_Code_Block_Destructor_Func__() noexcept; \
    static const int __Static_Code_Block_Destructor__ = []() -> int { \
        constexpr auto __postPriority__ = McPrivate::extractRoutinePriority(__VA_ARGS__); \
        Mc::addPostRoutine(__postPriority__, []() { __Static_Code_Block_Destructor_Func__(); }); \
        return 0; \
    }(); \
    static void __Static_Code_Block_Destructor_Func__() noexcept

#define MC_GLOBAL_STATIC_BEGIN(NAME) \
    namespace { \
        struct Mc_##NAME##_StaticData \
        {
#define MC_GLOBAL_STATIC_END(NAME) \
        }; \
        Q_GLOBAL_STATIC(Mc_##NAME##_StaticData, NAME) \
    }

#define MC_FULL_DEFINE_HELPER(...) , ##__VA_ARGS__
#define MC_FULL_DEFINE(Class, Parent, ...) \
    MC_DECL_INIT(Class) \
    MC_INTERFACES(Parent MC_FULL_DEFINE_HELPER(__VA_ARGS__)) \
    MC_DECL_SUPER(Parent)

#ifdef MC_USE_QT5
# define MC_DECL_METATYPE_NONE(Class) \
  Q_DECLARE_METATYPE(Class##Ptr) \
  Q_DECLARE_METATYPE(Class *)

# define MC_DECL_METATYPE(Class) \
  MC_DECL_POINTER(Class) \
  MC_DECL_METATYPE_NONE(Class)

# define MC_DECL_METATYPE_NS(Class, NS) \
  MC_DECL_POINTER_NS(Class, NS) \
  MC_DECL_METATYPE_NONE(NS::Class)
#else
# define MC_DECL_METATYPE(Class) MC_DECL_POINTER(Class)

# define MC_DECL_METATYPE_NS(Class, NS) MC_DECL_POINTER_NS(Class, NS)
#endif

#define MC_BASE_DESTRUCTOR(Class) virtual ~Class() = default;
#define MC_DEFINE_INTERFACE(Class) \
    public: \
    Class() noexcept = default; \
    MC_BASE_DESTRUCTOR(Class) \
    private: \
    Q_DISABLE_COPY_MOVE(Class)

#define MC_DECLARE_OPERATORS_FOR_FLAGS(Flags) \
    constexpr friend QFlags<Flags::enum_type> operator|(Flags::enum_type f1, Flags::enum_type f2) noexcept \
    { return QFlags<Flags::enum_type>(f1) | f2; } \
    constexpr friend QFlags<Flags::enum_type> operator|(Flags::enum_type f1, QFlags<Flags::enum_type> f2) noexcept \
    { return f2 | f1; } \
    constexpr friend QIncompatibleFlag operator|(Flags::enum_type f1, int f2) noexcept \
    { return QIncompatibleFlag(int(f1) | f2); }

//! PROPERTY
#define MC_POCO_PROPERTY(Type, name, ...) \
    Q_PROPERTY(Type name MEMBER name) \
    Type name __VA_ARGS__;
//!< PROPERTY

//! Q_CLASSINFO
#define MC_SERIALIZATION_TAG "McSerialization"
#define MC_JSON_SERIALIZATION_TAG "McJsonSerialization"

#define MC_JSON_SERIALIZATION() Q_CLASSINFO(MC_SERIALIZATION_TAG, MC_JSON_SERIALIZATION_TAG)
//!< Q_CLASSINFO
// clang-format on
