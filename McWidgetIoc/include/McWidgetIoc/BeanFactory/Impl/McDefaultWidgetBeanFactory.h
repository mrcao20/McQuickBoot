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

#include "McAbstractWidgetBeanFactory.h"

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

MC_FORWARD_DECL_CLASS(IMcPropertyConverter);
MC_FORWARD_DECL_CLASS(IMcBuiltInBeanBuilder)

MC_FORWARD_DECL_PRIVATE_DATA(McDefaultWidgetBeanFactory);

class MCWIDGETIOC_EXPORT McDefaultWidgetBeanFactory : public McAbstractWidgetBeanFactory
{
    Q_OBJECT
public:
    McDefaultWidgetBeanFactory(QObject *parent = nullptr);
    ~McDefaultWidgetBeanFactory() override;

    void setConverter(IMcPropertyConverterConstPtrRef converter) noexcept;

protected:
    QWidget *doCreate(IMcBeanDefinitionConstPtrRef beanDefinition) const noexcept override;

private:
    void callTagFunction(QObject *bean,
                         const char *tag,
                         Qt::ConnectionType type = Qt::DirectConnection) const noexcept;
    void callStartFunction(QObject *bean) const noexcept;
    bool addPropertyValue(QWidget *bean,
                          IMcBeanDefinitionConstPtrRef beanDefinition,
                          QVariantMap &proValues) const noexcept;
    void addBuiltInProperty(QWidget *bean,
                            IMcBuiltInBeanBuilderConstPtrRef builder,
                            IMcBeanDefinitionConstPtrRef beanDefinition) const noexcept;
    bool addObjectConnect(QObject *bean,
                          IMcBeanDefinitionConstPtrRef beanDefinition,
                          const QVariantMap &proValues) const noexcept;
    QObject *getPropertyObject(QObject *bean,
                               const QString &proName,
                               const QVariantMap &proValues) const noexcept;
    void callFinishedFunction(QObject *bean) const noexcept;
    void callThreadFinishedFunction(QObject *bean) noexcept;
    QVariant convert(const QVariant &value, QWidget *parent) const noexcept;
    QVariant convertRef(const QVariant &value, QWidget *parent) const noexcept;
    QVariant convertList(const QVariant &value, QWidget *parent) const noexcept;
    QVariant convertMap(const QVariant &value, QWidget *parent) const noexcept;

private:
    MC_DECL_PRIVATE(McDefaultWidgetBeanFactory)
};

MC_DECL_POINTER(McDefaultWidgetBeanFactory)
