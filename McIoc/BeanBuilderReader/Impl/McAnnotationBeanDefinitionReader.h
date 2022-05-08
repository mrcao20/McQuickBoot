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

#include "McAbstractBeanBuilderReader.h"

MC_FORWARD_DECL_CLASS(McBeanConnector)
MC_FORWARD_DECL_CLASS(McAbstractBeanBuilder)

MC_FORWARD_DECL_PRIVATE_DATA(McAnnotationBeanDefinitionReader)

class MC_IOC_EXPORT McAnnotationBeanDefinitionReader : public McAbstractBeanBuilderReader
{
public:
    explicit McAnnotationBeanDefinitionReader(
        const QVector<McMetaType> &metaTypes, const QMultiMap<QString, McBeanConnectorPtr> &beanConnectors) noexcept;
    ~McAnnotationBeanDefinitionReader();

protected:
    void doReadBeanBuilder() noexcept override;

private:
    void addMetaTypeBeanName(McMetaType type, const QString &beanName) noexcept;
    void addMetaTypeBeanName(const QSet<McMetaType> &types, const QString &beanName) noexcept;
    QSet<QString> getBeanNameForMetaType(McMetaType type) noexcept;

    QMap<QString, McMetaType> parseComponent() noexcept;
    void parseProperty(const McAbstractBeanBuilderPtr &builder, const QMetaObject *metaObj) noexcept;
    void parsePropertyOnAutowired(
        const McAbstractBeanBuilderPtr &builder, const QMetaObject *metaObj, const QString &value) noexcept;
    void parsePropertyOnResource(
        const McAbstractBeanBuilderPtr &builder, const QMetaObject *metaObj, const QByteArray &value) noexcept;

private:
    MC_DECL_PRIVATE(McAnnotationBeanDefinitionReader)
};

MC_DECL_POINTER(McAnnotationBeanDefinitionReader)
