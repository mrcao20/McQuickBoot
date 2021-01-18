#pragma once

#include <McIoc/BeanDefinition/IMcBeanDefinition.h>

class McSimpleBeanDefinition : public QObject, public IMcBeanDefinition
{
    Q_OBJECT
public:
    QVariant getBean() const noexcept override { return m_bean; }
    void setBean(const QVariant &bean) noexcept override { m_bean = bean; }

    bool isPointer() const noexcept override { return false; }
    void setPointer(bool val) noexcept override { Q_UNUSED(val) }

    bool isSingleton() const noexcept override { return true; }
    void setSingleton(bool val) noexcept override { Q_UNUSED(val) }

    const QMetaObject *getBeanMetaObject() const noexcept override { return nullptr; }
    void setBeanMetaObject(const QMetaObject *o) noexcept override { Q_UNUSED(o) }

    QString getClassName() const noexcept override { return ""; }
    void setClassName(const QString &name) noexcept override { Q_UNUSED(name) }

    QString getPluginPath() const noexcept override { return ""; }
    void setPluginPath(const QString &path) noexcept override { Q_UNUSED(path) }

    QVariantHash getProperties() const noexcept override { return QVariantHash(); }
    void addProperty(const QString &name, const QVariant &value) noexcept override
    {
        Q_UNUSED(name)
        Q_UNUSED(value)
    }

    QVariantList getConnectors() const noexcept override { return QVariantList(); }
    void addConnector(const QVariant &val) noexcept override { Q_UNUSED(val) }

private:
    QVariant m_bean;
};

MC_DECL_POINTER(McSimpleBeanDefinition)
