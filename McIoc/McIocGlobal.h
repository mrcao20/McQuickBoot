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

#include <QLoggingCategory>

#include <McCore/McGlobal.h>

#include "McIocConstantGlobal.h"
#include "McIocMacroGlobal.h"

Q_DECLARE_LOGGING_CATEGORY(mcIoc)

//MC_FORWARD_DECL_CLASS(IMcApplicationContext)

//! 如果你看到这行注释，则表示你可以不再需要使用这个宏，而是直接调用其函数
//! 此宏保留以适配以前大规模使用的老代码
#define MC_REGISTER_BEAN_FACTORY(Class, ...) mcRegisterMetaType<Class>();

#define MC_REGISTER_CONTAINER_CONVERTER(Type) \
    McPrivate::McContainerConverterRegisterHelper<Type>::registerConverter(#Type);
#define MC_REGISTER_LIST_CONVERTER MC_REGISTER_CONTAINER_CONVERTER
#define MC_REGISTER_MAP_CONVERTER MC_REGISTER_CONTAINER_CONVERTER

namespace McPrivate {

//! beanName应当用QByteArray
//QString getBeanName(const QMetaObject *metaObj) noexcept;

} // namespace McPrivate

namespace Mc {

////! 获取所有被Component标记的bean
//MC_IOC_EXPORT QList<QString> getAllComponent(IMcApplicationContextConstPtrRef appCtx) noexcept;
////! 获取所有组建类型为componentType的bean的名称
//MC_IOC_EXPORT QList<QString> getComponents(IMcApplicationContextConstPtrRef appCtx, const QString &componentType) noexcept;
////! 是否被Component标记
//MC_IOC_EXPORT bool isComponent(const QMetaObject *metaObj) noexcept;
////! 传入的元对象的组件类型是否为type
//MC_IOC_EXPORT bool isComponentType(const QMetaObject *metaObj, const QString &type) noexcept;
//MC_IOC_EXPORT bool isContainedTag(const QString &tags, const QString &tag) noexcept;
//MC_IOC_EXPORT QObject *getObject(IMcApplicationContext *appCtx, const QString &beanName) noexcept;

namespace Ioc {

//MC_IOC_EXPORT void connect(const QString &beanName,
//                          const QString &sender,
//                          const QString &signal,
//                          const QString &receiver,
//                          const QString &slot,
//                          Qt::ConnectionType type = Qt::AutoConnection) noexcept;

//MC_IOC_EXPORT void connect(const QMetaObject *metaObj,
//                          const QString &sender,
//                          const QString &signal,
//                          const QString &receiver,
//                          const QString &slot,
//                          Qt::ConnectionType type = Qt::AutoConnection) noexcept;

//MC_IOC_EXPORT QMetaObject::Connection connect(IMcApplicationContext *appCtx,
//                                             const QString &sender,
//                                             const QString &signal,
//                                             const QString &receiver,
//                                             const QString &slot,
//                                             Qt::ConnectionType type = Qt::AutoConnection) noexcept;

//MC_IOC_EXPORT QMetaObject::Connection connect(IMcApplicationContext *appCtx,
//                                             const QString &sender,
//                                             const QString &signal,
//                                             QObject *receiver,
//                                             const QString &slot,
//                                             Qt::ConnectionType type = Qt::AutoConnection) noexcept;

//MC_IOC_EXPORT bool disconnect(IMcApplicationContext *appCtx,
//                             const QString &sender,
//                             const QString &signal,
//                             const QString &receiver,
//                             const QString &slot) noexcept;

//MC_IOC_EXPORT bool disconnect(IMcApplicationContext *appCtx,
//                             const QString &sender,
//                             const QString &signal,
//                             QObject *receiver,
//                             const QString &slot) noexcept;

} // namespace Ioc
} // namespace Mc
