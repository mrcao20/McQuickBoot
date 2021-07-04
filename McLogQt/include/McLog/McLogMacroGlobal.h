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

#define MC_PRIVATE_DATA_NAME(Class) __Mc_##Class##Data__

#define MC_DECL_PRIVATE_DATA(Class) \
    struct MC_PRIVATE_DATA_NAME(Class) {

#define MC_DECL_PRIVATE_DATA_END };

#define MC_PRIVATE_DATA_DESTRUCTOR(Class) \
    ~MC_PRIVATE_DATA_NAME(Class)() {

#define MC_PRIVATE_DATA_DESTRUCTOR_END }

#define MC_DECL_PRIVATE(Class) \
    QScopedPointer<MC_PRIVATE_DATA_NAME(Class)> d; \
    friend struct MC_PRIVATE_DATA_NAME(Class);

#define MC_NEW_PRIVATE_DATA(Class) \
    d.reset(new MC_PRIVATE_DATA_NAME(Class)());

#define MC_FORWARD_DECL_PRIVATE_DATA(Class) \
    struct MC_PRIVATE_DATA_NAME(Class);

#define MC_PADDING_CLANG(size)  \
    char ___clang_padding___[size];

#define MC_DECL_INIT(Class) \
    static const int Class##_Static_Init;

#include <QLoggingCategory>

#define MC_LOGGER(name) QLoggingCategory(name)

#define MC_DEFAULT_LOGGER MC_LOGGER("default")

#define MC_PRINT_ERR(...) \
    fprintf(stderr, __VA_ARGS__); \
    fflush(stderr);
