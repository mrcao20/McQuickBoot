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

#define MC_STRINGIFY(x) #x

#define MC_SAFETY_DELETE(p) \
    { \
        if (p != nullptr) { \
            delete p; \
            p = nullptr; \
        } \
    }

#define MC_DECL_POINTER(Class) \
    using Class##Ptr = QSharedPointer<Class>; \
    using Class##ConstPtrRef = const QSharedPointer<Class> &;

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

#define MC_DECL_INIT(Class) static const int Class##_Static_Init;

#define MC_DECL_SUPER(SUPER) \
private: \
    typedef SUPER super;

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
        { \
            ClassType *_ = nullptr; \
            Q_UNUSED(_); \
        } \
        Mc::addPreRoutine(MC_ROUTINE_PRIORITY(__VA_ARGS__), [](){
#define MC_AUTO_INIT(Class, ...) \
    MC_INIT(Class, __VA_ARGS__) \
    MC_REGISTER_BEAN_FACTORY(Class)

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

#define MC_GLOBAL_STATIC_BEGIN(NAME) \
    namespace { \
    struct Mc_##NAME##_StaticData \
    {
#define MC_GLOBAL_STATIC_END(NAME) \
    } \
    ; \
    MC_GLOBAL_STATIC(Mc_##NAME##_StaticData, NAME) \
    }

#define MC_DECL_METATYPE_NONE(Class) \
    Q_DECLARE_METATYPE(Class##Ptr) \
    Q_DECLARE_METATYPE(Class *)

#define MC_DECL_METATYPE(Class) \
    MC_DECL_POINTER(Class) \
    MC_DECL_METATYPE_NONE(Class)

#define MC_DECL_METATYPE_NS(Class, NS) \
    MC_DECL_POINTER_NS(Class, NS) \
    MC_DECL_METATYPE_NONE(NS::Class)

#define MC_CONNECT(sender, signal, receiver, slot, ...) \
    Mc::Ioc::connect(&ClassType::staticMetaObject, sender, signal, receiver, slot, ##__VA_ARGS__)

#define MC_BASE_DESTRUCTOR(Class) virtual ~Class() = default;

#ifndef Q_MOC_RUN //!< 这行语句必须加，只有包围在这行语句之中的宏才能被识别为tag

#define MC_BEAN_START      //!< 丢弃，同MC_STARTED
#define MC_STARTED         //!< 当bean被构造，但还未注入属性时调用
#define MC_BEAN_FINISHED   //!< 丢弃，同MC_FINISHED
#define MC_FINISHED        //!< 当bean完全被构造完成之后调用
#define MC_THREAD_FINISHED //!< 丢弃，同MC_THREAD_MOVED
#define MC_THREAD_MOVED    //!< 当bean的线程被移动之后调用
//!< 注意：以上三个tag标记的函数调用线程为getBean时的线程
#define MC_ALL_FINISHED //!< 丢弃，同MC_COMPLETE
#define MC_COMPLETE //!< 当bean完全被构造之后，且线程移动之后调用，使用队列方式，调用线程回归到对象的生存线程

#endif //! !Q_MOC_RUN

//! Q_CLASSINFO
#define MC_COMPONENT_TAG "McComponent"

#define MC_SINGLETON_TAG "McIsSingleton"
#define MC_POINTER_TAG "McIsPointer"
#define MC_BEANNAME_TAG "McBeanName"
#define MC_AUTOWIRED_TAG "McAutowired"
#define MC_AUTOWIRED_SPLIT_SYMBOL "="
#define MC_RESOURCE_TAG "McResource"

#define MC_SINGLETON(arg) Q_CLASSINFO(MC_SINGLETON_TAG, MC_STRINGIFY(arg))
#define MC_POINTER(arg) Q_CLASSINFO(MC_POINTER_TAG, MC_STRINGIFY(arg))
#define MC_BEANNAME(name) Q_CLASSINFO(MC_BEANNAME_TAG, name)
#define MC_AUTOWIRED(v, ...) Q_CLASSINFO(MC_AUTOWIRED_TAG, v MC_AUTOWIRED_SPLIT_SYMBOL __VA_ARGS__)
//! 使用此宏注入容器类型时，Value只能是QSharedPointer类型。注入普通类型时既可以是原始指针，也可以是动态指针。
//! 且注入映射容器时，Key只能是QString，表示beanName
#define MC_RESOURCE(name) Q_CLASSINFO(MC_RESOURCE_TAG, name)
#define MC_COMPONENT(...) \
    MC_BEANNAME("" __VA_ARGS__) \
    Q_CLASSINFO(MC_COMPONENT_TAG, MC_COMPONENT_TAG)
//!< Q_CLASSINFO

//! PROPERTY
#define MC_PROPERTY(Type, name, ...) \
    MC_AUTOWIRED(#name) \
    Q_PROPERTY(Type name __VA_ARGS__)

#define MC_PROPERTY2(Type, name, ...) \
    MC_RESOURCE(#name) \
    Q_PROPERTY(Type name __VA_ARGS__)

#define MC_PRIVATE_PROPERTY(Type, name, ...) \
    MC_AUTOWIRED(#name) \
    Q_PRIVATE_PROPERTY(d, Type name __VA_ARGS__)

#define MC_PRIVATE_PROPERTY2(Type, name, ...) \
    MC_RESOURCE(#name) \
    Q_PRIVATE_PROPERTY(d, Type name __VA_ARGS__)

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
