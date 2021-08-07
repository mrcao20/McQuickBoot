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

#include "../IMcBeanDefinition.h"

class MCIOC_EXPORT McRootBeanDefinition : public QObject, public IMcBeanDefinition
{
    Q_OBJECT
public:
    QVariant getBean() const noexcept override 
    { return m_bean; }
    void setBean(const QVariant &bean) noexcept override { m_bean = bean; }

    bool isPointer() const noexcept override { return m_isPointer; }
    void setPointer(bool val) noexcept override { m_isPointer = val; }

    bool isSingleton() const noexcept override 
    { return m_isSingleton; }
    void setSingleton(bool val) noexcept override 
    { m_isSingleton = val; }

    const QMetaObject *getBeanMetaObject() const noexcept override 
    { return m_beanMetaObject; }
    void setBeanMetaObject(const QMetaObject *o) noexcept override 
    { m_beanMetaObject = o; }

    QString getClassName() const noexcept override 
    { return m_className; }
    void setClassName(const QString &name) noexcept override 
    {
        m_className = name;
        if(m_beanMetaObject != nullptr) {
            return;
        }
        auto type = QMetaType::type(m_className.toLocal8Bit());
        if(type == QMetaType::UnknownType) {
            if (m_className.endsWith("*")) {
                return;
            }
            type = QMetaType::type((m_className + "*").toLocal8Bit());
            if(type == QMetaType::UnknownType) {
                return;
            }
        }
        m_beanMetaObject = QMetaType::metaObjectForType(type);
    }

    QString getPluginPath() const noexcept override 
    { return m_pluginPath; }
    void setPluginPath(const QString &path) noexcept override 
    { m_pluginPath = path; }

    QVariantHash getProperties() const noexcept override 
    { return m_properties; }
    void addProperty(const QString &name, const QVariant &value) noexcept override
    { m_properties.insert(name, value); }
    
    QVariantList getConnectors() const noexcept override 
    { return m_connectors; }
    void addConnector(const QVariant &val) noexcept override 
    { m_connectors.append(val); }

private:
    QVariant m_bean;                                    //!< 包含bean的QVariant。此对象不再删除该bean
    bool m_isPointer{false};                            //!< 是否为指针类型，默认否
    bool m_isSingleton{true};                           //!< 该bean是否是单例，默认是
    const QMetaObject *m_beanMetaObject{ nullptr };     //!< bean的MetaObject对象
    QString m_className;                                //!< bean的类全限定名称
    QString m_pluginPath;                               //!< bean的插件路径
    QVariantHash m_properties;                            //!< bean的属性集合
    QVariantList m_connectors;                          //!< bean中需要连接的信号槽
};

MC_DECL_POINTER(McRootBeanDefinition)
