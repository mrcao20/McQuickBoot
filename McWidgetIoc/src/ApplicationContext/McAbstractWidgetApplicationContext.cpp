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
#include "McWidgetIoc/ApplicationContext/Impl/McAbstractWidgetApplicationContext.h"

MC_DECL_PRIVATE_DATA(McAbstractWidgetApplicationContext)
IMcConfigurableWidgetBeanFactoryPtr configurableBeanFactory;
MC_DECL_PRIVATE_DATA_END

McAbstractWidgetApplicationContext::McAbstractWidgetApplicationContext(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractWidgetApplicationContext);
}

McAbstractWidgetApplicationContext::~McAbstractWidgetApplicationContext() {}

QWidget *McAbstractWidgetApplicationContext::getBean(const QString &name, QWidget *parent) const
    noexcept
{
    return d->configurableBeanFactory->getBean(name, parent);
}

bool McAbstractWidgetApplicationContext::containsBean(const QString &name) const noexcept
{
    return d->configurableBeanFactory->containsBean(name);
}

bool McAbstractWidgetApplicationContext::registerBeanDefinition(
    const QString &name, IMcBeanDefinitionConstPtrRef beanDefinition) noexcept
{
    return d->configurableBeanFactory->registerBeanDefinition(name, beanDefinition);
}

IMcBeanDefinitionPtr McAbstractWidgetApplicationContext::unregisterBeanDefinition(
    const QString &name) noexcept
{
    return d->configurableBeanFactory->unregisterBeanDefinition(name);
}

bool McAbstractWidgetApplicationContext::canRegister(
    IMcBeanDefinitionConstPtrRef beanDefinition) const noexcept
{
    return d->configurableBeanFactory->canRegister(beanDefinition);
}

bool McAbstractWidgetApplicationContext::isContained(const QString &name) const noexcept
{
    return containsBean(name);
}

QHash<QString, IMcBeanDefinitionPtr> McAbstractWidgetApplicationContext::getBeanDefinitions() const
    noexcept
{
    return d->configurableBeanFactory->getBeanDefinitions();
}

void McAbstractWidgetApplicationContext::setBeanFactory(
    IMcConfigurableWidgetBeanFactoryConstPtrRef factory) noexcept
{
    d->configurableBeanFactory = factory;
}
