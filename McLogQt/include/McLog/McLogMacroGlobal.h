#pragma once

#include <QtCore/qglobal.h>

#if defined (MC_BUILD_STATIC) && !defined (MC_EXPORT_DISABLE)
#define MC_EXPORT_DISABLE
#endif

#ifndef MC_EXPORT_DISABLE
# if defined(MCLOGQT_LIBRARY)
#  define MCLOGQT_EXPORT Q_DECL_EXPORT
# else
#  define MCLOGQT_EXPORT Q_DECL_IMPORT
# endif
#else
# define MCLOGQT_EXPORT
#endif

#ifdef MC_DECL_POINTER
#undef MC_DECL_POINTER
#endif
#ifdef MC_FORWARD_DECL_CLASS
#undef MC_FORWARD_DECL_CLASS
#endif
#ifdef MC_FORWARD_DECL_STRUCT
#undef MC_FORWARD_DECL_STRUCT
#endif
#ifdef MC_PRIVATE_DATA_NAME
#undef MC_PRIVATE_DATA_NAME
#endif
#ifdef MC_DECL_PRIVATE_DATA
#undef MC_DECL_PRIVATE_DATA
#endif
#ifdef MC_DECL_PRIVATE_DATA_END
#undef MC_DECL_PRIVATE_DATA_END
#endif
#ifdef MC_DECL_PRIVATE
#undef MC_DECL_PRIVATE
#endif
#ifdef MC_NEW_PRIVATE_DATA
#undef MC_NEW_PRIVATE_DATA
#endif
#ifdef MC_FORWARD_DECL_PRIVATE_DATA
#undef MC_FORWARD_DECL_PRIVATE_DATA
#endif
#ifdef MC_PADDING_CLANG
#undef MC_PADDING_CLANG
#endif
#ifdef MC_DECL_INIT
#undef MC_DECL_INIT
#endif

#define MC_DECL_POINTER(Class) \
    using Class##Ptr = QSharedPointer<Class>; \
    using Class##ConstPtrRef = const QSharedPointer<Class> &;

#define MC_FORWARD_DECL_CLASS(Class) \
    class Class; \
    MC_DECL_POINTER(Class)

#define MC_FORWARD_DECL_STRUCT(Class) \
    struct Class; \
    using Class##Ptr = QSharedPointer<Class>; \
    using Class##ConstPtrRef = const QSharedPointer<Class> &;

#define MC_PRIVATE_DATA_NAME(Class) \
    Class##Data

#define MC_DECL_PRIVATE_DATA(Class) \
    struct Class##Data {

#define MC_DECL_PRIVATE_DATA_END };

#define MC_DECL_PRIVATE(Class) \
    QScopedPointer<Class##Data> d; \
    friend struct Class##Data;

#define MC_NEW_PRIVATE_DATA(Class) \
    d.reset(new Class##Data());

#define MC_FORWARD_DECL_PRIVATE_DATA(Class) \
    struct Class##Data;

#define MC_PADDING_CLANG(size)  \
    char ___clang_padding___[size];

#define MC_DECL_INIT(Class) \
    static const int Class##_Static_Init;

#ifdef MC_NO_IOC

# define MC_BEAN_START
# define MC_BEAN_FINISHED
# define MC_THREAD_FINISHED

#define MC_DECL_METATYPE(Class) \
    MC_DECL_POINTER(Class)

#define MC_INIT(...)
#define MC_INIT_END
#define MC_TYPELIST(...)
#define MC_REGISTER_BEAN_FACTORY(...)
#define MC_REGISTER_LIST_CONVERTER(...)
#define MC_REGISTER_MAP_CONVERTER(...)

#endif

#include <QLoggingCategory>

#define MC_LOGGER(name) QLoggingCategory(name)

#define MC_DEFAULT_LOGGER MC_LOGGER("default")

#define MC_PRINT_ERR(...) \
    fprintf(stderr, __VA_ARGS__); \
    fflush(stderr);
