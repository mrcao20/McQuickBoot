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
