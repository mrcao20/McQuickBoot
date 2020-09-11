#pragma once

#include <QtCore/qglobal.h>

#if defined(MCLOGQT_LIBRARY)
#  define MCLOGQT_EXPORT Q_DECL_EXPORT
#else
#  define MCLOGQT_EXPORT Q_DECL_IMPORT
#endif

#ifdef MC_NO_IOC

#define MC_DECL_POINTER(Class) \
    using Class##Ptr = QSharedPointer<Class>; \
    using Class##ConstPtrRef = const QSharedPointer<Class> &;

#define MC_FORWARD_DECL_CLASS(Class) \
    class Class; \
    MC_DECL_POINTER(Class)

#define MC_DECL_PRIVATE_DATA(Class) \
    struct Class##Data {

#define MC_DECL_PRIVATE_DATA_END };

#define MC_DECL_PRIVATE(Class) \
    QScopedPointer<Class##Data> d;

#define MC_NEW_PRIVATE_DATA(Class) \
    d.reset(new Class##Data());

#define MC_FORWARD_DECL_PRIVATE_DATA(Class) \
    struct Class##Data;

#define MC_PADDING_CLANG(size)  \
    char ___clang_padding___[size];

# define MC_BEAN_START
# define MC_BEAN_FINISHED
# define MC_THREAD_FINISHED

#define MC_DECL_METATYPE(Class) \
    MC_DECL_POINTER(Class)

#define MC_DECL_INIT(...)
#define MC_INIT(...)
#define MC_INIT_END
#define MC_DEFINE_TYPELIST(...)
#define MC_DECL_TYPELIST(...)
#define MC_TYPELIST(...) MC_DECL_TYPELIST(__VA_ARGS__)
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
