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

#include "../IMcWidgetApplicationContext.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractWidgetApplicationContext);

class MCWIDGETIOC_EXPORT McAbstractWidgetApplicationContext : public QObject,
                                                              public IMcWidgetApplicationContext
{
    Q_OBJECT
public:
    using IMcWidgetBeanFactory::getBean;

    McAbstractWidgetApplicationContext(QObject *parent = nullptr);
    ~McAbstractWidgetApplicationContext() override;

    QWidget *getBean(const QString &name, QWidget *parent = nullptr) const noexcept override;
    bool containsBean(const QString &name) const noexcept override;
    bool registerBeanDefinition(const QString &name,
                                IMcBeanDefinitionConstPtrRef beanDefinition) noexcept override;
    bool registerBeanDefinition(const QHash<QString, IMcBeanDefinitionPtr> &vals) noexcept override;
    IMcBeanDefinitionPtr unregisterBeanDefinition(const QString &name) noexcept override;
    bool canRegister(IMcBeanDefinitionConstPtrRef beanDefinition) const noexcept override;
    bool isContained(const QString &name) const noexcept override;
    QHash<QString, IMcBeanDefinitionPtr> getBeanDefinitions() const noexcept override;

protected:
    void setBeanFactory(IMcConfigurableWidgetBeanFactoryConstPtrRef factory) noexcept;

private:
    MC_DECL_PRIVATE(McAbstractWidgetApplicationContext)
};

MC_DECL_POINTER(McAbstractWidgetApplicationContext)
