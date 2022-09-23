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

#include <QtCore/qglobal.h>

#include "McConfig.h"

#define MC_QUICKBOOT_VERSION \
 QT_VERSION_CHECK(MC_QUICKBOOT_VERSION_MAJOR, MC_QUICKBOOT_VERSION_MINOR, MC_QUICKBOOT_VERSION_PATCH)

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

#define MC_DECL_POINTER(Class) using Class##Ptr = QSharedPointer<Class>;

#define MC_DECL_POINTER_NS(Class, NS) \
 namespace NS { \
 MC_DECL_POINTER(Class) \
 }

#define MC_FORWARD_DECL_CLASS(Class) \
 class Class; \
 MC_DECL_POINTER(Class)

#define MC_FORWARD_DECL_CLASS_NS(Class, NS) \
 namespace NS { \
 MC_FORWARD_DECL_CLASS(Class) \
 }

#define MC_FORWARD_DECL_STRUCT(Class) \
 struct Class; \
 MC_DECL_POINTER(Class)

#define MC_FORWARD_DECL_STRUCT_NS(Class, NS) \
 namespace NS { \
 MC_FORWARD_DECL_STRUCT(Class) \
 }

#define MC_PRIVATE_DATA_NAME(Class) __Mc_##Class##Data__

#define MC_DECL_PRIVATE_DATA(Class) \
 struct MC_PRIVATE_DATA_NAME(Class) \
 {
#define MC_DECL_PRIVATE_DATA_END \
 } \
 ;

#define MC_PRIVATE_DATA_DESTRUCTOR(Class) \
 ~MC_PRIVATE_DATA_NAME(Class)() \
 {
//
#define MC_PRIVATE_DATA_DESTRUCTOR_END }

#define MC_DECL_PRIVATE(Class) \
 QScopedPointer<MC_PRIVATE_DATA_NAME(Class)> d; \
 friend struct MC_PRIVATE_DATA_NAME(Class);

#define MC_DECL_COPYABLE_PRIVATE(Class) \
 std::unique_ptr<MC_PRIVATE_DATA_NAME(Class)> d; \
 friend struct MC_PRIVATE_DATA_NAME(Class);

#define MC_NEW_PRIVATE_DATA(Class) d.reset(new MC_PRIVATE_DATA_NAME(Class)());

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
 } \
 ();

#define MC_STATIC(...) \
 namespace { \
 static const int __Static_Code_Block__ = []() -> int { \
        constexpr auto __prePriority__ = McPrivate::extractRoutinePriority(__VA_ARGS__); \
        Mc::addPreRoutine(__prePriority__, [](){
#define MC_STATIC_END \
 }); \
 return 0; \
 } \
 (); \
 }

#define MC_GLOBAL_STATIC_BEGIN(NAME) \
 namespace { \
 struct Mc_##NAME##_StaticData \
 {
#define MC_GLOBAL_STATIC_END(NAME) \
 } \
 ; \
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
