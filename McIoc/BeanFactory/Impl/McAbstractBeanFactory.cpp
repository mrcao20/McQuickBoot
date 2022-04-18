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
#include "McAbstractBeanFactory.h"

#include <QDebug>
#include <QMutex>

#include "BeanBuilder/IMcBeanBuilder.h"
#include "BeanBuilder/Impl/McBeanReference.h"

MC_DECL_PRIVATE_DATA(McAbstractBeanFactory)
QHash<QString, IMcBeanBuilderPtr> hash;
QRecursiveMutex mtx;
QThread *targetThread{nullptr}; //!< 生成对象的目标线程
MC_DECL_PRIVATE_DATA_END

McAbstractBeanFactory::McAbstractBeanFactory(QObject *parent) noexcept
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractBeanFactory);
}

McAbstractBeanFactory::~McAbstractBeanFactory() {}

QVariant McAbstractBeanFactory::getBeanToVariant(const QString &name, QThread *thread) noexcept
{
    QMutexLocker locker(&d->mtx);
    d->targetThread = thread;
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

QObjectPtr McAbstractBeanFactory::resolveBeanReference(const McBeanReferencePtr &beanRef) noexcept
{
    return resolveBeanReferenceToQVariant(beanRef).value<QObjectPtr>();
}

QObject *McAbstractBeanFactory::resolveBeanReferencePointer(const McBeanReferencePtr &beanRef) noexcept
{
    return resolveBeanReferenceToQVariant(beanRef).value<QObject *>();
}

QVariant McAbstractBeanFactory::resolveBeanReferenceToQVariant(const McBeanReferencePtr &beanRef) noexcept
{
    if (beanRef.isNull()) {
        return QVariant();
    }
    return getBeanToVariant(beanRef->name(), d->targetThread);
}

void McAbstractBeanFactory::beanReferenceMoveToThread(const McBeanReferencePtr &beanRef, QThread *thread) noexcept
{
    if (beanRef.isNull()) {
        return;
    }
    moveToThread(beanRef->name(), thread);
}
