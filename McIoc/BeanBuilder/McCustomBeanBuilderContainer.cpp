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
#include "McCustomBeanBuilderContainer.h"

namespace {
using BuilderFactoryType = QHash<QString, IMcCustomBeanBuilderFactoryPtr>;
Q_GLOBAL_STATIC(BuilderFactoryType, staticBeanBuilderFactories)
} // namespace

void McCustomBeanBuilderContainer::addBuilderFactory(
    const QString &className, const IMcCustomBeanBuilderFactoryPtr &factory) noexcept
{
    staticBeanBuilderFactories->insert(className, factory);
}

McAbstractBeanBuilderPtr McCustomBeanBuilderContainer::getBuilder(const QString &className) noexcept
{
    auto factory = staticBeanBuilderFactories->value(className);
    if (factory.isNull()) {
        return McAbstractBeanBuilderPtr();
    }
    return factory->create();
}

bool McCustomBeanBuilderContainer::containsBuilder(const QString &className) noexcept
{
    return staticBeanBuilderFactories->contains(className);
}
