/*
 * Copyright (c) 2021 mrcao20/mrcao20@163.com
 * McQuickBoot is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
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
