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
MC_FORWARD_DECL_CLASS(QSettings);
QT_END_NAMESPACE

MC_FORWARD_DECL_CLASS(IMcBeanDefinition);

MC_FORWARD_DECL_PRIVATE_DATA(McSettingBeanDefinitionReader);

class MCIOC_EXPORT McSettingBeanDefinitionReader : public McAbstractBeanDefinitionReader 
{
    Q_OBJECT
    using McAbstractBeanDefinitionReader::readBeanDefinition;
public:
    explicit McSettingBeanDefinitionReader(
        QSettingsConstPtrRef setting, QObject *parent = nullptr);
    explicit McSettingBeanDefinitionReader(
        const QList<QSettingsPtr> &settings, QObject *parent = nullptr);
    virtual ~McSettingBeanDefinitionReader() override;

protected:
    void doReadBeanDefinition() noexcept override;
    
private:
    IMcBeanDefinitionPtr buildBeanDefinition(QSettingsConstPtrRef setting) noexcept;
    void readBeanDefinitionForProperty(QSettingsConstPtrRef setting,
                                       IMcBeanDefinitionConstPtrRef beanDefinition) noexcept;
    void readBeanDefinitionForProperty(const QMap<QString, QVariant> &map,
                                       IMcBeanDefinitionConstPtrRef beanDefinition) noexcept;
    void readBeanDefinitionForConnect(QSettingsConstPtrRef setting,
                                      IMcBeanDefinitionConstPtrRef beanDefinition) noexcept;
    void readBeanDefinitionForConnect(const QMap<QString, QVariant> &map,
                                      IMcBeanDefinitionConstPtrRef beanDefinition) noexcept;
    QVariant parseProperty(const QVariant &var) noexcept;
    QString getUnregisteredBeanName(const QString &proName, const QString &className) const noexcept;

private:
    MC_DECL_PRIVATE(McSettingBeanDefinitionReader)
};

MC_DECL_POINTER(McSettingBeanDefinitionReader)
