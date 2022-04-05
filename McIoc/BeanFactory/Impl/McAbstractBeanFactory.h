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

#include "../IMcBeanReferenceResolver.h"
#include "../IMcConfigurableBeanFactory.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractBeanFactory);

class MC_IOC_EXPORT McAbstractBeanFactory : public QObject,
                                            public IMcConfigurableBeanFactory,
                                            public IMcBeanReferenceResolver
{
    Q_OBJECT
public:
    using IMcBeanFactory::getBean;
    using IMcBeanFactory::getBeanPointer;

    explicit McAbstractBeanFactory(QObject *parent = nullptr) noexcept;
    ~McAbstractBeanFactory() override;

    QVariant getBeanToVariant(const QString &name, QThread *thread = nullptr) noexcept override;
    void moveToThread(const QString &name, QThread *thread) noexcept override;
    bool containsBean(const QString &name) const noexcept override;
    bool isSingleton(const QString &name) const noexcept override;
    bool isPointer(const QString &name) const noexcept override;

    bool registerBeanBuilder(const QString &name, const IMcBeanBuilderPtr &beanBuilder) noexcept override;
    bool registerBeanBuilder(const QHash<QString, IMcBeanBuilderPtr> &vals) noexcept override;
    IMcBeanBuilderPtr unregisterBeanBuilder(const QString &name) noexcept override;
    bool isContained(const QString &name) const noexcept override;
    QHash<QString, IMcBeanBuilderPtr> getBeanBuilders() const noexcept override;

    QObjectPtr resolveBeanReference(const McBeanReferencePtr &beanRef) noexcept override;
    QObject *resolveBeanReferencePointer(const McBeanReferencePtr &beanRef) noexcept override;
    QVariant resolveBeanReferenceToQVariant(const McBeanReferencePtr &beanRef) noexcept override;
    void beanReferenceMoveToThread(const McBeanReferencePtr &beanRef, QThread *thread) noexcept override;

protected:
    virtual void afterBuildBean(const QVariant &bean) noexcept = 0;

private:
    MC_DECL_PRIVATE(McAbstractBeanFactory)
};

MC_DECL_POINTER(McAbstractBeanFactory)
