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
#include "McInvoker.h"

#include <QThreadPool>

#include <McCore/Utils/McRequest.h>

#include "McResult.h"

namespace McPrivate {
#ifdef MC_USE_QT5
static bool buildArguments(const QList<int> &metaTypes, const QVariantList &originArgs, McRequest request,
    QVariantList &dstArgs, QVariant &errMsg) noexcept
#else
static bool buildArguments(const QList<QMetaType> &metaTypes, const QVariantList &originArgs, McRequest request,
    QVariantList &dstArgs, QVariant &errMsg) noexcept
#endif
{
#ifdef MC_USE_QT5
    if (metaTypes.size() == 1 && metaTypes.constFirst() == qMetaTypeId<McRequest>()) {
#else
    if (metaTypes.size() == 1 && metaTypes.constFirst() == QMetaType::fromType<McRequest>()) {
#endif
        request.setParams(originArgs);
        dstArgs.append(QVariant::fromValue(request));
    } else if (metaTypes.size() == 1 && McPrivate::isContainedCustomRequest(metaTypes.constFirst())) {
        auto mainMetaType = metaTypes.constFirst();
        auto childMetaTypes = McPrivate::getCustomRequestMetaType(mainMetaType);
        if (originArgs.size() < childMetaTypes.size()) {
            errMsg = fail(
                QStringLiteral("Incoming argument size must be greater than or equal to functor argument size"));
            return false;
        }
        QVariantList reqArgs;
        for (qsizetype i = 0; i < childMetaTypes.size(); ++i) {
            auto type = childMetaTypes.at(i);
            QVariant value = originArgs.at(i); //!< 由于调用此函数时参数个数一定一致，所以这里一定有值
            if (!value.convert(type)) {
#ifdef MC_USE_QT5
                errMsg = fail(QString("property convert failure. origin type name: %1. typeid: %2. "
                                      "target typeName: %3")
                                  .arg(value.typeName(), QString::number(value.type()), QMetaType::typeName(type)));
#else
                errMsg = fail(QStringLiteral("property convert failure. origin type name: %1. typeid: %2. "
                                             "target typeName: %3")
                                  .arg(value.typeName(), QString::number(value.typeId()), type.name()));
#endif
                return false;
            }
            reqArgs.append(value);
        }
        dstArgs.append(McPrivate::buildCustomRequest(mainMetaType, reqArgs, request));
    } else {
        if (originArgs.size() < metaTypes.size()) {
            errMsg = fail(
                QStringLiteral("Incoming argument size must be greater than or equal to functor argument size"));
            return false;
        }
        for (qsizetype i = 0; i < metaTypes.size(); ++i) {
            auto type = metaTypes.at(i);
            QVariant value = originArgs.at(i); //!< 由于调用此函数时参数个数一定一致，所以这里一定有值
            if (!value.convert(type)) {
#ifdef MC_USE_QT5
                errMsg = fail(QString("property convert failure. origin type name: %1. typeid: %2. "
                                      "target typeName: %3")
                                  .arg(value.typeName(), QString::number(value.type()), QMetaType::typeName(type)));
#else
                errMsg = fail(QStringLiteral("property convert failure. origin type name: %1. typeid: %2. "
                                             "target typeName: %3")
                                  .arg(value.typeName(), QString::number(value.typeId()), type.name()));
#endif
                return false;
            }
            dstArgs.append(value);
        }
    }
    return true;
}

#ifdef MC_USE_QT5
static QVariant syncInvoke_helper(const McRequest &request, const McSlotObjectWrapper &functor,
    const QList<int> &metaTypes, const QVariantList &originArgs) noexcept
#else
static QVariant syncInvoke_helper(const McRequest &request, const McSlotObjectWrapper &functor,
    const QList<QMetaType> &metaTypes, const QVariantList &originArgs) noexcept
#endif
{
    QVariantList dstArgs;
    QVariant body;
    if (buildArguments(metaTypes, originArgs, request, dstArgs, body)) {
        try {
            body = functor.call(dstArgs);
        } catch (const std::exception &e) {
            body = fail(e.what());
        }
    }
    return body;
}

QVariant fail(const QString &val) noexcept
{
    auto result = McResult::fail(val);
    result->setInternalError(true);
    return QVariant::fromValue(result);
}

#ifdef MC_USE_QT5
QVariant syncInvoke(
    const McSlotObjectWrapper &functor, const QList<int> &metaTypes, const QVariantList &arguments) noexcept
#else
QVariant syncInvoke(
    const McSlotObjectWrapper &functor, const QList<QMetaType> &metaTypes, const QVariantList &arguments) noexcept
#endif
{
    return syncInvoke_helper(McRequest(), functor, metaTypes, arguments);
}

#ifdef MC_USE_QT5
void invoke(McAbstractPromise *promise, const McSlotObjectWrapper &functor, const QList<int> &metaTypes,
    const QVariantList &arguments) noexcept
#else
void invoke(McAbstractPromise *promise, const McSlotObjectWrapper &functor, const QList<QMetaType> &metaTypes,
    const QVariantList &arguments) noexcept
#endif
{
    auto runThreadFunc = [promise, functor, metaTypes, arguments]() {
        Mc::globalThreadPool()->start(
            [promise = QPointer<McAbstractPromise>(promise), functor, metaTypes, originArgs = arguments]() {
                //! 注意此lambda表达式不能随意return，必须确保promise正确的析构
                McRequest request;
                if (!promise.isNull()) {
                    promise->setStarted();
                    promise->setTargetThread(QThread::currentThread());
                    request.setCancel(promise->getCancel());
                    request.setPause(promise->getPause());
                    request.setProgress(promise->getProgress());
                    promise->setRunning();
                }

                QVariant body = syncInvoke_helper(request, functor, metaTypes, originArgs);

                if (promise.isNull()) { //!< promise可能被QML析构
                    qCCritical(mcQuickBoot, "promise is null. it's maybe destroyed of qmlengine");
                    return;
                }
                promise->setRunning(false);
                //! setBody之中会调用deleteLater
                promise->setBody(body);
            });
    };
    QMetaObject::invokeMethod(promise, runThreadFunc, Qt::QueuedConnection);
}
} // namespace McPrivate
