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

#include "../IMcConfigurableApplicationContext.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractApplicationContext);

class MCIOC_EXPORT McAbstractApplicationContext : public QObject,
                                                  public IMcConfigurableApplicationContext
{
    Q_OBJECT
public:
    using IMcBeanFactory::getBean;
    using IMcBeanFactory::getBeanPointer;

    McAbstractApplicationContext(IMcConfigurableBeanFactoryConstPtrRef factory,
                                 QObject *parent = nullptr);
    ~McAbstractApplicationContext() override;

    IMcPropertyConverterPtr getPropertyConverter() const noexcept override;
    void setPropertyConverter(IMcPropertyConverterConstPtrRef converter) noexcept override;

    QObjectPtr getBean(const QString &name, QThread *thread = nullptr) noexcept override;
    QObject *getBeanPointer(const QString &name, QThread *thread = nullptr) noexcept override;
    QVariant getBeanToVariant(const QString &name, QThread *thread = nullptr)  noexcept override;
    bool containsBean(const QString &name) const noexcept override;
    bool isSingleton(const QString &name) noexcept override;
    bool registerBeanDefinition(const QString &name,
                                IMcBeanDefinitionConstPtrRef beanDefinition) noexcept override;
    bool registerBeanDefinition(const QHash<QString, IMcBeanDefinitionPtr> &vals) noexcept override;
    IMcBeanDefinitionPtr unregisterBeanDefinition(const QString &name) noexcept override;
    bool canRegister(IMcBeanDefinitionConstPtrRef beanDefinition) const noexcept override;
    bool isContained(const QString &name) const noexcept override;
    QHash<QString, IMcBeanDefinitionPtr> getBeanDefinitions() const noexcept override;
    void refresh(QThread *thread = nullptr) noexcept override;
    
    void addRelatedBeanFactory(IMcConfigurableBeanFactoryConstPtrRef beanFac) noexcept override;
    void removeRelatedBeanFactory(IMcConfigurableBeanFactoryConstPtrRef beanFac) noexcept override;
    QList<IMcConfigurableBeanFactoryPtr> getRelatedBeanFactories() noexcept override;

private:
    MC_DECL_PRIVATE(McAbstractApplicationContext)
};

MC_DECL_POINTER(McAbstractApplicationContext)
