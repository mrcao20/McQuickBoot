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
#include "McIoc/BeanFactory/impl/McPointerBeanFactory.h"

#include <QDebug>

#include "McIoc/BeanDefinition/IMcBeanDefinition.h"
#include "McIoc/BeanFactory/impl/McBeanReference.h"
#include "McIoc/Thread/IMcDeleteThreadWhenQuit.h"

MC_DECL_PRIVATE_DATA(McPointerBeanFactory)
MC_DECL_PRIVATE_DATA_END

McPointerBeanFactory::McPointerBeanFactory(QObject *parent) noexcept
    : McAbstractNormalBeanFactory(parent)
{
    MC_NEW_PRIVATE_DATA(McPointerBeanFactory);
}

McPointerBeanFactory::~McPointerBeanFactory() {}

bool McPointerBeanFactory::canRegister(IMcBeanDefinitionConstPtrRef beanDefinition) const noexcept
{
    return beanDefinition->isPointer();
}

bool McPointerBeanFactory::canResolve(McBeanReferenceConstPtrRef beanRef) noexcept
{
    if (!beanRef) {
        qCritical() << "beanReference not exists";
        return false;
    }
    auto pluginPath = beanRef->getPluginPath();
    if (!pluginPath.isEmpty()) {
        return beanRef->isPointer();
    } else {
        auto defs = getBeanDefinitions();
        if (!defs.contains(beanRef->getName())) {
            return false;
        }
        return canRegister(defs.value(beanRef->getName()));
    }
}

QVariant McPointerBeanFactory::convertToQVariant(QObject *obj) noexcept
{
    QVariant var;
    var.setValue(obj);
    QString typeName = QString("%1*").arg(obj->metaObject()->className());
    if (!var.convert(QMetaType::type(typeName.toLocal8Bit()))) {
        qCritical() << QString("failed convert QObject* to '%1'").arg(typeName);
        return QVariant();
    }
    auto destoryer = var.value<IMcDeleteThreadWhenQuitPtr>();
    if (destoryer) {
        destoryer->deleteWhenQuit();
    }
    return var;
}

QVariant McPointerBeanFactory::convertToQVariant(void *gadget, const QMetaObject *metaObj) noexcept
{
    QByteArray clazzName = metaObj->className();
    clazzName.append('*');
    QVariant var(QMetaType::type(clazzName), &gadget);
    return var;
}
