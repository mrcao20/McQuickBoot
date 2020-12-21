#pragma once

#include <QtCore/qglobal.h>

#include "Utils/Macro/MacroSize.h"
#include "Utils/Macro/MacroFillingFunc.h"

#if defined (MC_BUILD_STATIC) && !defined (MC_EXPORT_DISABLE)
#define MC_EXPORT_DISABLE
#endif

#ifndef MC_EXPORT_DISABLE
# if defined(MCIOC_LIBRARY)
#  define MCIOC_EXPORT Q_DECL_EXPORT
# else
#  define MCIOC_EXPORT Q_DECL_IMPORT
# endif
#else
# define MCIOC_EXPORT
#endif

#define MC_MACRO_STR(m) #m

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

#define MC_FILLING_ROUTINE_PRIORITY_FUNC_0() Mc::RoutinePriority::Normal
#define MC_FILLING_ROUTINE_PRIORITY_FUNC_1(args) args
#define MC_FUNC_CHOOSER(_f1, _f2, ...) _f2
#define MC_FUNC_RECOMPOSER(par) MC_FUNC_CHOOSER par
#define MC_CHOOSE_FROM_ARG_COUNT(...) MC_FUNC_RECOMPOSER((__VA_ARGS__, MC_FILLING_ROUTINE_PRIORITY_FUNC_1,))
#define MC_NO_ARG_EXPANDER() ,MC_FILLING_ROUTINE_PRIORITY_FUNC_0
#define MC_MACRO_CHOOSER(...) MC_CHOOSE_FROM_ARG_COUNT(MC_NO_ARG_EXPANDER __VA_ARGS__ ())
#define MC_ROUTINE_PRIORITY(...) MC_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#define MC_INIT(Class, ...) \
    const int Class::Class##_Static_Init = []() -> int { \
        using ClassType = Class; \
        Mc::addPreRoutine(MC_ROUTINE_PRIORITY(__VA_ARGS__), [](){
#define MC_AUTO_INIT(Class, ...) \
    MC_INIT(Class, __VA_ARGS__) \
    MC_REGISTER_BEAN_FACTORY(MC_TYPELIST(Class))

#define MC_DESTROY(...) \
    }); \
    Mc::addPostRoutine(MC_ROUTINE_PRIORITY(__VA_ARGS__), [](){

#define MC_INIT_END \
    }); \
    return 0;}();

#define MC_STATIC(...) \
    namespace { \
    static const int __Static_Code_Block__ = []() -> int { \
        Mc::addPreRoutine(MC_ROUTINE_PRIORITY(__VA_ARGS__), [](){

#define MC_STATIC_END \
        }); \
        return 0; \
    }(); \
    }

#define MC_CONNECT(sender, signal, receiver, slot, ...) \
    Mc::Ioc::connect(&ClassType::staticMetaObject, sender, signal, receiver, slot, __VA_ARGS__)


#ifndef Q_MOC_RUN            //!< 这行语句必须加，只有包围在这行语句之中的宏才能被识别为tag

# define MC_BEAN_START      //!< 当bean被构造，但还未注入属性时调用
# define MC_BEAN_FINISHED   //!< 当bean完全被构造完成之后调用
# define MC_THREAD_FINISHED   //!< 当bean的线程被移动之后调用

#endif //! !Q_MOC_RUN

//! Q_CLASSINFO
#define MC_COMPONENT_TAG "Component"

#define MC_SINGLETON_TAG "isSingleton"
#define MC_BEANNAME_TAG "beanName"
#define MC_AUTOWIRED_TAG "autowired"

#define MC_COMPONENT Q_CLASSINFO(MC_COMPONENT_TAG, MC_COMPONENT_TAG)
#define MC_SINGLETON(arg) Q_CLASSINFO(MC_SINGLETON_TAG, MC_MACRO_STR(arg))
#define MC_BEANNAME(name) Q_CLASSINFO(MC_BEANNAME_TAG, name)
#define MC_AUTOWIRED(v) Q_CLASSINFO(MC_AUTOWIRED_TAG, v)
//!< Q_CLASSINFO

//! PROPERTY
#define MC_POCO_PROPERTY(Type, name, ...) \
    Q_PROPERTY(Type name MEMBER name) \
    Type name __VA_ARGS__;
//!< PROPERTY

#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
#define qInfo QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).debug
#endif

//! QT相关宏的重定义
#define MC_GLOBAL_STATIC Q_GLOBAL_STATIC
//!<
