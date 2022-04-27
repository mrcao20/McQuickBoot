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
#include "McResolverBeanFactory.h"

#include "BeanBuilder/IMcBeanBuilder.h"
#include "BeanBuilder/Impl/McBeanReference.h"

MC_DECL_PRIVATE_DATA(McResolverBeanFactory)
QThread *targetThread{nullptr}; //!< 生成对象的目标线程
MC_DECL_PRIVATE_DATA_END

McResolverBeanFactory::McResolverBeanFactory() noexcept
{
    MC_NEW_PRIVATE_DATA(McResolverBeanFactory);
}

McResolverBeanFactory::~McResolverBeanFactory() {}

QVariant McResolverBeanFactory::getBean(const QString &name, QThread *thread) noexcept
{
    d->targetThread = thread;
    return super::getBean(name, thread);
}

bool McResolverBeanFactory::registerBeanBuilder(const QString &name, const IMcBeanBuilderPtr &beanBuilder) noexcept
{
    if (beanBuilder.isNull()) {
        return false;
    }
    beanBuilder->setReferenceResolver(this);
    return super::registerBeanBuilder(name, beanBuilder);
}

QVariant McResolverBeanFactory::resolveBeanReference(const McBeanReferencePtr &beanRef) noexcept
{
    if (beanRef.isNull()) {
        return QVariant();
    }
    return getBean(beanRef->name(), d->targetThread);
}

void McResolverBeanFactory::beanReferenceMoveToThread(const McBeanReferencePtr &beanRef, QThread *thread) noexcept
{
    if (beanRef.isNull()) {
        return;
    }
    moveToThread(beanRef->name(), thread);
}

void McResolverBeanFactory::afterBuildBean(const QVariant &bean) noexcept
{
    Q_UNUSED(bean)
}
