#pragma once

#include "../IMcConfigurableWidgetBeanFactory.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractWidgetBeanFactory);

class MCWIDGETIOC_EXPORT McAbstractWidgetBeanFactory : public QObject,
                                                       public IMcConfigurableWidgetBeanFactory
{
    Q_OBJECT
public:
    using IMcWidgetBeanFactory::getBean;

    explicit McAbstractWidgetBeanFactory(QObject *parent = nullptr);
    ~McAbstractWidgetBeanFactory() override;

    QWidget *getBean(const QString &name, QWidget *parent = nullptr) const noexcept override;

    bool containsBean(const QString &name) const noexcept override;
    /*!
     * \brief registerBeanDefinition
     * 
     * 向容器中注册bean定义。
     * \param name beanName
     * \param beanDefinition bean的定义
     */
    bool registerBeanDefinition(const QString &name,
                                IMcBeanDefinitionConstPtrRef beanDefinition) noexcept override;

    IMcBeanDefinitionPtr unregisterBeanDefinition(const QString &name) noexcept override;

    /*!
     * \brief isContained
     * 
     * 是否已经注册过该bean
     * \param name beanName
     * \return 是否已经注册过
     * \retval true 已经注册过
     * \retval false 没有被注册
     */
    bool isContained(const QString &name) const noexcept override;

    bool canRegister(IMcBeanDefinitionConstPtrRef beanDefinition) const noexcept override;
    /*!
     * \brief getBeanDefinitions
     * 
     * 获取所有bean定义的集合
     * \return bean定义的集合。
     * \see IMcBeanDefinitionRegistry
     */
    QHash<QString, IMcBeanDefinitionPtr> getBeanDefinitions() const noexcept override;

protected:
    virtual QWidget *doCreate(IMcBeanDefinitionConstPtrRef beanDefinition) const noexcept = 0;

private:
    MC_DECL_PRIVATE(McAbstractWidgetBeanFactory)
};

MC_DECL_POINTER(McAbstractWidgetBeanFactory)
