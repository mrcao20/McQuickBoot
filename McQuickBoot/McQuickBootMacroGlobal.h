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

#include <McIoc/McIocMacroGlobal.h>

#ifndef MC_EXPORT_DISABLE
# if defined(MC_QUICKBOOT_LIBRARY)
#  define MC_QUICKBOOT_EXPORT Q_DECL_EXPORT
# else
#  define MC_QUICKBOOT_EXPORT Q_DECL_IMPORT
# endif
#else
# define MC_QUICKBOOT_EXPORT
#endif

// clang-format off
#ifndef Q_MOC_RUN            //!< 这行语句必须加，只有包围在这行语句之中的宏才能被识别为tag

//! 声明式bean
#define MC_BEAN
//!< end

#endif //! !Q_MOC_RUN

//! Q_CLASSINFO
#define MC_CONTROLLER_TAG "McController"
#define MC_MODEL_TAG "McModel"
#define MC_SERVICE_TAG "McService"
#define MC_CONFIGURATION_TAG "McConfiguration"
#define MC_CONFIGURATION_PRIORITY_TAG "McConfigurationPriority"
#define MC_CONFIGURATION_PROPERTIES_TAG "McConfigurationProperties"
#define MC_PROPERTY_SOURCE_TAG "McPropertySource"
#define MC_SERIALIZATION_TAG "McSerialization"
#define MC_JSON_SERIALIZATION_TAG "McJsonSerialization"

#define MC_CONTROLLER(...) \
    MC_BEANNAME("" __VA_ARGS__) \
    Q_CLASSINFO(MC_COMPONENT_TAG, MC_CONTROLLER_TAG)
#define MC_MODEL(...) \
    MC_BEANNAME("" __VA_ARGS__) \
    Q_CLASSINFO(MC_COMPONENT_TAG, MC_MODEL_TAG)
#define MC_SERVICE(...) \
    MC_BEANNAME("" __VA_ARGS__) \
    Q_CLASSINFO(MC_COMPONENT_TAG, MC_SERVICE_TAG)
#define MC_CONFIGURATION(...) \
    Q_CLASSINFO(MC_COMPONENT_TAG, MC_CONFIGURATION_TAG) \
    Q_CLASSINFO(MC_CONFIGURATION_PRIORITY_TAG, MC_STRINGIFY("@" __VA_ARGS__))
#define MC_CONFIGURATION_PROPERTIES(arg) Q_CLASSINFO(MC_CONFIGURATION_PROPERTIES_TAG, arg)
#define MC_PROPERTY_SOURCE(arg) Q_CLASSINFO(MC_PROPERTY_SOURCE_TAG, arg)
#define MC_JSON_SERIALIZATION() Q_CLASSINFO(MC_SERIALIZATION_TAG, MC_JSON_SERIALIZATION_TAG)
//!< Q_CLASSINFO

// Work Thread
#define MC_QUICKBOOT_WORK_THREAD_NAME __mc__quickBootWorkThread
#define MC_INLINE_QUICKBOOT_WORK_THREAD() \
    Q_INVOKABLE \
    MC_BEAN \
    QThread *MC_QUICKBOOT_WORK_THREAD_NAME() const noexcept { return new QThread(); }

#define MC_DECL_QUICKBOOT_WORK_THREAD() \
    Q_INVOKABLE \
    MC_BEAN \
    QThread *MC_QUICKBOOT_WORK_THREAD_NAME() const noexcept;

#define MC_SIMPLE_DEFINE_QUICKBOOT_WORK_THREAD(Class) \
    QThread *Class::MC_QUICKBOOT_WORK_THREAD_NAME() const noexcept { return new QThread(); }

#define MC_CUSTOM_DEFINE_QUICKBOOT_WORK_THREAD(Class) \
    QThread *Class::MC_QUICKBOOT_WORK_THREAD_NAME() const noexcept

#define MC_CUSTOM_FUNC_DEFINE_QUICKBOOT_WORK_THREAD(Class, func) \
    QThread *Class::MC_QUICKBOOT_WORK_THREAD_NAME() const noexcept { return func(); }
// clang-format on
