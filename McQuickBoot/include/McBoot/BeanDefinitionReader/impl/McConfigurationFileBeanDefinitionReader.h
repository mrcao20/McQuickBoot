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

#include <McIoc/BeanDefinitionReader/impl/McAbstractBeanDefinitionReader.h>

MC_FORWARD_DECL_CLASS(IMcBeanDefinition);
MC_FORWARD_DECL_CLASS(IMcApplicationContext);
MC_FORWARD_DECL_CLASS(QSettings);

MC_FORWARD_DECL_PRIVATE_DATA(McConfigurationFileBeanDefinitionReader);

class McConfigurationFileBeanDefinitionReader 
        : public McAbstractBeanDefinitionReader 
{
    Q_OBJECT
public:
    McConfigurationFileBeanDefinitionReader(
            IMcApplicationContextConstPtrRef appCtx
            , QObject *parent = nullptr);
    ~McConfigurationFileBeanDefinitionReader() override;

    static void addConfigPath(const QString &path) noexcept;

protected:
    void doReadBeanDefinition() noexcept override;
    
private:
    QString getDefaultConfigPath() const noexcept;
    void copyBeanDefinition(QSettingsConstPtrRef setting
                            , const QString &path, const QString &prefix
                            , const QMetaObject *metaObj) noexcept;
    void copyBeanDefinition(QSettingsConstPtrRef setting
                            , QSettings &originSet, const QMetaObject *metaObj) noexcept;
    QVariant buildChildProperty(const QString &proName
                                , const QVariant &value, const QMetaObject *metaObj) noexcept;
    
private:
    MC_DECL_PRIVATE(McConfigurationFileBeanDefinitionReader)
};

MC_DECL_POINTER(McConfigurationFileBeanDefinitionReader)
