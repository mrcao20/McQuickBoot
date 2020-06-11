#pragma once

#include "../IMcConfigurableBeanFactory.h"
#include "../IMcBeanReferenceResolver.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractBeanFactory);

class MCIOCCONTAINER_EXPORT McAbstractBeanFactory
        : public QObject
        , public IMcConfigurableBeanFactory
        , public IMcBeanReferenceResolver 
{
    
    Q_OBJECT
public:
    using IMcBeanFactory::getBean;
    
    explicit McAbstractBeanFactory(QObject *parent = nullptr);
    ~McAbstractBeanFactory() override;

    /*!
     * \brief getBean
     * 
     * 根据bean的名称获取bean，如果没有，则返回空。
     * 如果有，则从bean定义对象获取bean实例。
     * \param name beanName
     * \param thread 将要生存的目标线程
     * \return 返回获取到的对象
     */
    QObjectPtr getBean(const QString &name, QThread *thread) noexcept override;
    /*!
     * \brief getBeanToVariant
     * 
     * 根据bean的名称获取包含bean实例的QVariant，如果没有，则返回无效值。
     * 如果有，则从bean定义对象获取包含bean实例的QVariant。
     * \param name beanName
     * \param thread 将要生存的目标线程
     * \return 
     */
    QVariant getBeanToVariant(const QString &name, QThread *thread)  noexcept override;
    /*!
     * \brief containsBean
     * 
     * 检测容器中是否存在该bean。
     * \param name beanName
     * \return 是否存在
     * \retval true 存在
     * \retval false 不存在
     */
    bool containsBean(const QString &name) noexcept override;
    /*!
     * \brief isSingleton
     * 
     * 检查bean是否为单例
     * \param name
     * \return 
     */
    bool isSingleton(const QString &name) noexcept override;
    /*!
     * \brief registerBeanDefinition
     * 
     * 向容器中注册bean定义。
     * \param name beanName
     * \param beanDefinition bean的定义
     */
    void registerBeanDefinition(const QString &name, IMcBeanDefinitionConstPtrRef beanDefinition) noexcept override;
    /*!
     * \brief isContained
     * 
     * 是否已经注册过该bean
     * \param name beanName
     * \return 是否已经注册过
     * \retval true 已经注册过
     * \retval false 没有被注册
     */
    bool isContained(const QString &name) noexcept override;
    /*!
     * \brief getBeanDefinitions
     * 
     * 获取所有bean定义的集合
     * \return bean定义的集合。
     * \see IMcBeanDefinitionRegistry
     */
    QHash<QString, IMcBeanDefinitionPtr> getBeanDefinitions() noexcept override;
    /*!
     * \brief resolveBeanReference
     * 
     * 根据beanReference生成实例对象。
     * \see IMcBeanReferenceResolver
     * \param beanRef
     * \return 
     */
    QObjectPtr resolveBeanReference(McBeanReferenceConstPtrRef beanRef) noexcept override;
    /*!
     * \brief resolveBeanReferenceToQVariant
     * 
     * 根据beanReference生成包含实例对象的QVariant。
     * \see IMcBeanReferenceResolver
     * \param beanRef
     * \return 
     */
    QVariant resolveBeanReferenceToQVariant(McBeanReferenceConstPtrRef beanRef) noexcept override;
    
protected:
    /*!
     * \brief doCreate
     * 
     * 根据传入的bean定义和目标线程创建一个bean
     * \param beanDefinition bean定义
     * \param thread 目标生存线程
     * \return 创建的bean的包裹QVariant
     * \note 此函数会返回无效值
     */
    virtual QVariant doCreate(IMcBeanDefinitionConstPtrRef beanDefinition, QThread *thread) noexcept = 0;

private:
    MC_DECL_PRIVATE(McAbstractBeanFactory)
};

MC_DECL_POINTER(McAbstractBeanFactory)
