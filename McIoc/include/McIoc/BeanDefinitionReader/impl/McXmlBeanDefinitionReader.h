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

#include "McAbstractBeanDefinitionReader.h"

QT_BEGIN_NAMESPACE
MC_FORWARD_DECL_CLASS(QIODevice);
class QDomDocument;
class QDomNodeList;
class QDomElement;
QT_END_NAMESPACE

MC_FORWARD_DECL_CLASS(IMcBeanDefinition)
MC_FORWARD_DECL_CLASS(IMcPropertyParser)

MC_FORWARD_DECL_PRIVATE_DATA(McXmlBeanDefinitionReader)

class MCIOC_EXPORT McXmlBeanDefinitionReader : public McAbstractBeanDefinitionReader 
{
    Q_OBJECT
    using McAbstractBeanDefinitionReader::readBeanDefinition;
public:
    explicit McXmlBeanDefinitionReader(IMcPropertyParserConstPtrRef parser,
                                       QIODeviceConstPtrRef device,
                                       const QString &flag = QString(),
                                       QObject *parent = nullptr);
    explicit McXmlBeanDefinitionReader(IMcPropertyParserConstPtrRef parser,
                                       const QList<QIODevicePtr> &devices,
                                       const QString &flag = QString(),
                                       QObject *parent = nullptr);
    virtual ~McXmlBeanDefinitionReader() override;

protected:
    void doReadBeanDefinition() noexcept override;

private:
    void readBeanDefinition(QIODeviceConstPtrRef source) noexcept;
    void readBeanDefinition(const QDomDocument &doc) noexcept;
    void readBeanDefinition(const QDomNodeList &nodes) noexcept;
    bool parseBeanClass(const QDomElement &ele, IMcBeanDefinitionConstPtrRef beanDefinition) noexcept;
    void readBeanDefinition(const QDomNodeList &propNodes,
                            IMcBeanDefinitionConstPtrRef beanDefinition) noexcept;
    void readBeanDefinitionForProperty(const QDomElement &propEle,
                                       IMcBeanDefinitionConstPtrRef beanDefinition) noexcept;
    void readBeanDefinitionForConnect(const QDomElement &propEle,
                                      IMcBeanDefinitionConstPtrRef beanDefinition) noexcept;

private:
    MC_DECL_PRIVATE(McXmlBeanDefinitionReader)
};

MC_DECL_POINTER(McXmlBeanDefinitionReader)
