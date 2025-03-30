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
#include "McAbstractBeanFactory.h"

#include <QDebug>
#include <QMutex>

#include "BeanBuilder/IMcBeanBuilder.h"

MC_DECL_PRIVATE_DATA(McAbstractBeanFactory)
QHash<QString, IMcBeanBuilderPtr> hash;
QRecursiveMutex mtx;
MC_DECL_PRIVATE_DATA_END

McAbstractBeanFactory::McAbstractBeanFactory() noexcept
{
    MC_NEW_PRIVATE_DATA(McAbstractBeanFactory);
}

McAbstractBeanFactory::~McAbstractBeanFactory() {}

QVariant McAbstractBeanFactory::getBean(const QString &name, QThread *thread) noexcept
{
    QMutexLocker locker(&d->mtx);
    IMcBeanBuilderPtr beanBuilder = d->hash.value(name);
    if (beanBuilder.isNull()) {
        return QVariant();
    }
    QVariant bean = beanBuilder->build(thread);
    if (!bean.isValid()) {
        qCCritical(mcIoc(), "'%s' build failed.", qPrintable(name));
        return QVariant();
    }
    afterBuildBean(bean);
    return bean;
}

void McAbstractBeanFactory::moveToThread(const QString &name, QThread *thread) noexcept
{
    QMutexLocker locker(&d->mtx);
    IMcBeanBuilderPtr beanBuilder = d->hash.value(name);
    if (beanBuilder.isNull()) {
        return;
    }
    beanBuilder->moveToThread(thread);
}

bool McAbstractBeanFactory::containsBean(const QString &name) const noexcept
{
    QMutexLocker locker(&d->mtx);
    return d->hash.contains(name);
}

bool McAbstractBeanFactory::isSingleton(const QString &name) const noexcept
{
    QMutexLocker locker(&d->mtx);
    auto beanBuilder = d->hash.value(name);
    if (beanBuilder.isNull()) {
        return true;
    }
    return beanBuilder->isSingleton();
}

bool McAbstractBeanFactory::isPointer(const QString &name) const noexcept
{
    QMutexLocker locker(&d->mtx);
    auto beanBuilder = d->hash.value(name);
    if (beanBuilder.isNull()) {
        return false;
    }
    return beanBuilder->isPointer();
}

bool McAbstractBeanFactory::registerBeanBuilder(const QString &name, const IMcBeanBuilderPtr &beanBuilder) noexcept
{
    if (beanBuilder.isNull()) {
        return false;
    }
    QMutexLocker locker(&d->mtx);
    if (d->hash.contains(name)) {
        qCWarning(mcIoc(), "'%s' is already registered. replaced", qPrintable(name));
    }
    d->hash.insert(name, beanBuilder);
    return true;
}

bool McAbstractBeanFactory::registerBeanBuilder(const QHash<QString, IMcBeanBuilderPtr> &vals) noexcept
{
    bool ret = true;
    QHashIterator<QString, IMcBeanBuilderPtr> itr(vals);
    while (itr.hasNext()) {
        auto item = itr.next();
        if (!registerBeanBuilder(item.key(), item.value())) {
            ret = false;
        }
    }
    return ret;
}

IMcBeanBuilderPtr McAbstractBeanFactory::unregisterBeanBuilder(const QString &name) noexcept
{
    QMutexLocker locker(&d->mtx);
    return d->hash.take(name);
}

bool McAbstractBeanFactory::isContained(const QString &name) const noexcept
{
    return containsBean(name);
}

QHash<QString, IMcBeanBuilderPtr> McAbstractBeanFactory::getBeanBuilders() const noexcept
{
    QMutexLocker locker(&d->mtx);
    return d->hash;
}
