#pragma once

#include "../McGlobal.h"

#include <QVariant>

MC_BEGIN_NAMESPACE

class IMcBeanDefinition {
public:
    virtual ~IMcBeanDefinition() = default;

    virtual QVariant getBean() const noexcept = 0;
    virtual void setBean(const QVariant &bean) noexcept = 0;

    virtual bool isSingleton() const noexcept = 0;
    virtual void setSingleton(bool val) noexcept = 0;

	virtual const QMetaObject *getBeanMetaObject() const noexcept = 0;
	virtual void setBeanMetaObject(const QMetaObject *o) noexcept = 0;

	virtual QString getClassName() const noexcept = 0;
    /*!
     * \brief setClassName
     * 
     * 使用此函数设置className之后，会自动生成QMetaObject
     * \param name
     */
	virtual void setClassName(const QString &name) noexcept = 0;

    virtual QString getPluginPath() const noexcept = 0;
    virtual void setPluginPath(const QString &path) noexcept = 0;

	virtual QVariantHash getProperties() const noexcept = 0;
    /*!
     * \brief addProperty
     * 
     * 由于QVariant在析构时会自动删除包含的对象，所以该对象不用设置父对象
     * \param name
     * \param value
     */
	virtual void addProperty(const QString &name, const QVariant &value) noexcept = 0;
    
    virtual QVariantList getConnectors() const noexcept = 0;
    virtual void addConnector(const QVariant &val) noexcept = 0;
};

MC_END_NAMESPACE

MC_FORWARD_DECL_CLASS(IMcBeanDefinition)
