#pragma once

#include "../IMcBeanDefinition.h"

class McRootBeanDefinition : public QObject, public IMcBeanDefinition
{
    Q_OBJECT
public:
    QVariant getBean() const noexcept override 
    { return m_bean; }
    void setBean(const QVariant& bean) noexcept override 
    { m_bean = bean; }

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
            if(m_className.endsWith("*")) {
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
    bool m_isSingleton{true};                           //!< 该bean是否是单例，默认是
    const QMetaObject *m_beanMetaObject{ nullptr };     //!< bean的MetaObject对象
    QString m_className;                                //!< bean的类全限定名称
    QString m_pluginPath;                               //!< bean的插件路径
    QVariantHash m_properties;                            //!< bean的属性集合
    QVariantList m_connectors;                          //!< bean中需要连接的信号槽
};

MC_DECL_POINTER(McRootBeanDefinition)
