#pragma once

#include "McAbstractBeanFactory.h"

MC_FORWARD_DECL_PRIVATE_DATA(McDefaultBeanFactory);

MC_FORWARD_DECL_CLASS(IMcPropertyConverter);

class MCIOC_EXPORT McDefaultBeanFactory : public McAbstractBeanFactory 
{
    Q_OBJECT
public:
    explicit McDefaultBeanFactory(IMcPropertyConverterConstPtrRef converter,
                                  QObject *parent = nullptr);
    ~McDefaultBeanFactory() override;
    
protected:
    /*!
     * \brief doCreate
     * 
     * 根据bean定义创建实例，并指定目标线程。如果目标线程为空则bean的生存线程为调用
     * 此函数时的线程
     * \param beanDefinition bean定义
     * \param thread 目标线程
     * \return 包裹实例化出的bean的QVariant。可能为无效值
     */
    QVariant doCreate(IMcBeanDefinitionConstPtrRef beanDefinition,
                      QThread *thread) noexcept override;
    
private:
    /*!
     * \brief callTagFunction
     * 
     * 调用被某一个tag声明的函数
     * \param bean
     * \param tag
     */
    void callTagFunction(QObjectConstPtrRef bean, const char *tag) noexcept;
    /*!
     * \brief callStartFunction
     * 
     * 调用构造开始函数
     * \param bean
     */
    void callStartFunction(QObjectConstPtrRef bean) noexcept;
    /*!
     * \brief addPropertyValue
     * 
     * 给定一个bean定义和一个bean实例，为给定的bean中的属性注入实例。
     * 并将获取到的属性值设置给proValues
     * \param bean bean实例
     * \param beanDefinition bean定义
     * \param proValues 获取到的属性值
     * \return 是否全部成功
     */
    bool addPropertyValue(QObjectConstPtrRef bean,
                          IMcBeanDefinitionConstPtrRef beanDefinition,
                          QVariantMap &proValues) noexcept;
    
    /*!
     * \brief addObjectConnect
     * 
     * 为bean添加对应信号槽
     * \param bean bean实例
     * \param beanDefinition bean定义
     * \param proValues bean中的属性
     * \return 是否全部成功
     */
    bool addObjectConnect(QObjectConstPtrRef bean,
                          IMcBeanDefinitionConstPtrRef beanDefinition,
                          const QVariantMap &proValues) noexcept;
    
    /*!
     * \brief getPropertyObject
     * \param bean
     * 
     * 查找并获取将要连接的属性是否存在
     * \param proName 将要连接的属性名
     * \param proValues bean中已经注入的属性
     * \return 获取到的属性。可能为空
     */
    QObjectPtr getPropertyObject(QObjectConstPtrRef bean,
                                 const QString &proName,
                                 const QVariantMap &proValues) noexcept;
    
    /*!
     * \brief callFinishedFunction
     * 
     * 调用构造完成函数
     * \param bean
     */
    void callFinishedFunction(QObjectConstPtrRef bean) noexcept;
    /*!
     * \brief callThreadFinishedFunction
     * 
     * 调用线程移动结束函数，如果线程未被移动，则不会调用
     * \param bean
     */
    void callThreadFinishedFunction(QObjectConstPtrRef bean) noexcept;
    
private:
    MC_DECL_PRIVATE(McDefaultBeanFactory)
};

MC_DECL_POINTER(McDefaultBeanFactory)
