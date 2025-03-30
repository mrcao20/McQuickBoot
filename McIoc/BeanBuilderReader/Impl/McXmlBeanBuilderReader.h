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

class QXmlStreamReader;

MC_FORWARD_DECL_CLASS(QIODevice)
MC_FORWARD_DECL_CLASS(McAbstractBeanBuilder)
MC_FORWARD_DECL_CLASS(McGadgetBeanBuilder)
MC_FORWARD_DECL_CLASS(McObjectBeanBuilder)
MC_FORWARD_DECL_CLASS(McObjectClassBeanBuilder)
MC_FORWARD_DECL_CLASS(McObjectPluginBeanBuilder)
MC_FORWARD_DECL_CLASS(McBeanReference)

MC_FORWARD_DECL_PRIVATE_DATA(McXmlBeanBuilderReader)

class MC_IOC_EXPORT McXmlBeanBuilderReader : public McAbstractBeanBuilderReader
{
public:
    McXmlBeanBuilderReader(const QIODevicePtr &device, const QString &flag = QString()) noexcept;
    McXmlBeanBuilderReader(const QList<QIODevicePtr> &devices, const QString &flag = QString()) noexcept;
    ~McXmlBeanBuilderReader();

protected:
    void doReadBeanBuilder() noexcept override;

    virtual McObjectClassBeanBuilderPtr createObjectBeanBuilder(McMetaType metaType, bool isPointer) const noexcept;

private:
    void read(QXmlStreamReader &reader) const noexcept;
    QString readBean(QXmlStreamReader &reader) const noexcept;
    McAbstractBeanBuilderPtr readClass(
        QXmlStreamReader &reader, QStringView className, const McBeanReferencePtr &ref, bool isPointer) const noexcept;
    McAbstractBeanBuilderPtr readPlugin(
        QXmlStreamReader &reader, QStringView pluginPath, const McBeanReferencePtr &ref, bool isPointer) const noexcept;
    void readObjectBean(const McObjectClassBeanBuilderPtr &builder, QXmlStreamReader &reader) const noexcept;
    void readGadgetBean(const McGadgetBeanBuilderPtr &builder, QXmlStreamReader &reader) const noexcept;
    void readProperty(const McAbstractBeanBuilderPtr &builder, QXmlStreamReader &reader) const noexcept;
    void readConstructor(
        const McAbstractBeanBuilderPtr &builder, int autoArgIndex, QXmlStreamReader &reader) const noexcept;
    void readConnect(const McObjectBeanBuilderPtr &builder, QXmlStreamReader &reader) const noexcept;
    void readPluginChecker(const McObjectPluginBeanBuilderPtr &builder, QXmlStreamReader &reader) const noexcept;
    McAbstractBeanBuilderPtr readList(QXmlStreamReader &reader) const noexcept;
    McAbstractBeanBuilderPtr readMap(QXmlStreamReader &reader) const noexcept;
    QVariantList readListValue(QXmlStreamReader &reader) const noexcept;
    QMap<QVariant, QVariant> readMapValue(QXmlStreamReader &reader) const noexcept;
    QPair<QVariant, QVariant> readMapEntry(QXmlStreamReader &reader) const noexcept;
    QVariant readMapEntryKey(QXmlStreamReader &reader) const noexcept;
    QVariant readMapEntryValue(QXmlStreamReader &reader) const noexcept;

    McObjectPluginBeanBuilderPtr createPluginBeanBuilder(const QString &pluginPath, bool isPointer) const noexcept;
    QVariant readValue(QXmlStreamReader &reader) const noexcept;
    QVariantList getList(const QString &dirPath, bool isPointer) const noexcept;

    QString genarateUniqueName() const noexcept;

private:
    MC_DECL_PRIVATE(McXmlBeanBuilderReader)
};

MC_DECL_POINTER(McXmlBeanBuilderReader)
