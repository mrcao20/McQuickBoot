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

#if defined(MC_BUILD_STATIC) && !defined(MC_EXPORT_DISABLE)
# define MC_EXPORT_DISABLE
#endif

#ifndef MC_EXPORT_DISABLE
# if defined(MC_IOC_LIBRARY)
#  define MC_IOC_EXPORT Q_DECL_EXPORT
# else
#  define MC_IOC_EXPORT Q_DECL_IMPORT
# endif
#else
# define MC_IOC_EXPORT
#endif

#ifndef Q_MOC_RUN //!< 这行语句必须加，只有包围在这行语句之中的宏才能被识别为tag

# define MC_STARTED //!< 当bean被构造，但还未注入属性时调用
# define MC_FINISHED //!< 当bean完全被构造完成之后调用
# define MC_THREAD_MOVED //!< 当bean的线程被移动之后调用
//!< 注意：以上三个tag标记的函数调用线程为getBean时的线程
# define MC_COMPLETED //!< 当bean完全被构造之后，且线程移动之后调用，使用队列方式，调用线程回归到对象的生存线程

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
//! 注入映射容器时，Key只能是QString，表示beanName
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
//!< PROPERTY
