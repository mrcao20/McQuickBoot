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

#include <McCore/McMacroGlobal.h>

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

// clang-format off
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
// clang-format on
