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
#include "McAbstractApplicationContext.h"

#include "BeanBuilder/IMcBeanBuilder.h"
#include "BeanBuilder/Impl/McBeanReference.h"
#include "BeanFactory/IMcConfigurableBeanFactory.h"

MC_DECL_PRIVATE_DATA(McAbstractApplicationContext)
IMcConfigurableBeanFactoryPtr configurableBeanFactory;
QThread *targetThread{nullptr}; //!< 生成对象的目标线程
MC_DECL_PRIVATE_DATA_END

McAbstractApplicationContext::McAbstractApplicationContext(const IMcConfigurableBeanFactoryPtr &factory) noexcept
{
    MC_NEW_PRIVATE_DATA(McAbstractApplicationContext);
    d->configurableBeanFactory = factory;
}

McAbstractApplicationContext::~McAbstractApplicationContext() {}

QVariant McAbstractApplicationContext::getBean(const QString &name, QThread *thread) noexcept
{
    d->targetThread = thread;
    return d->configurableBeanFactory->getBean(name, thread);
}

void McAbstractApplicationContext::moveToThread(const QString &name, QThread *thread) noexcept
{
    d->configurableBeanFactory->moveToThread(name, thread);
}

bool McAbstractApplicationContext::containsBean(const QString &name) const noexcept
{
    return d->configurableBeanFactory->containsBean(name);
}

bool McAbstractApplicationContext::isSingleton(const QString &name) const noexcept
{
    return d->configurableBeanFactory->isSingleton(name);
}

bool McAbstractApplicationContext::isPointer(const QString &name) const noexcept
{
    return d->configurableBeanFactory->isPointer(name);
}

bool McAbstractApplicationContext::registerBeanBuilder(
    const QString &name, const IMcBeanBuilderPtr &beanBuilder) noexcept
{
    if (beanBuilder.isNull()) {
        return false;
    }
    beanBuilder->setReferenceResolver(this);
    return d->configurableBeanFactory->registerBeanBuilder(name, beanBuilder);
}

bool McAbstractApplicationContext::registerBeanBuilder(const QHash<QString, IMcBeanBuilderPtr> &vals) noexcept
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

IMcBeanBuilderPtr McAbstractApplicationContext::unregisterBeanBuilder(const QString &name) noexcept
{
    return d->configurableBeanFactory->unregisterBeanBuilder(name);
}

bool McAbstractApplicationContext::isContained(const QString &name) const noexcept
{
    return d->configurableBeanFactory->isContained(name);
}

QHash<QString, IMcBeanBuilderPtr> McAbstractApplicationContext::getBeanBuilders() const noexcept
{
    return d->configurableBeanFactory->getBeanBuilders();
}

void McAbstractApplicationContext::refresh(QThread *thread) noexcept
{
    auto beanBuilders = getBeanBuilders();
    QHashIterator<QString, IMcBeanBuilderPtr> itr(beanBuilders);
    while (itr.hasNext()) {
        auto item = itr.next();
        if (!item.value()->isSingleton()) { //!< 如果不是单例对象则不需要获取，因为非单例对象无法预存
            continue;
        }
        //!< 获取一次bean，让bean预加载
        getBean(item.key(), thread);
    }
}

QVariant McAbstractApplicationContext::resolveBeanReference(const McBeanReferencePtr &beanRef) noexcept
{
    if (beanRef.isNull()) {
        return QVariant();
    }
    return getBean(beanRef->name(), d->targetThread);
}

void McAbstractApplicationContext::beanReferenceMoveToThread(const McBeanReferencePtr &beanRef, QThread *thread) noexcept
{
    if (beanRef.isNull()) {
        return;
    }
    moveToThread(beanRef->name(), thread);
}
