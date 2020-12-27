#pragma once

#include "../McGlobal.h"

#include <QVariant>

QT_BEGIN_NAMESPACE
class QThread;
QT_END_NAMESPACE

class IMcBeanFactory 
{
public:
    virtual ~IMcBeanFactory() = default;
    
    /*!
     * \brief 根据传入的参数和类型返回一个QSharedPointer的bean
     * \see getBeanToVariant
     * \note 此函数并不符合接口的设计，因为C++模板不能作用在纯虚函数上，
     * 提供此函数仅用于提供一种方便的向目标类型转换的方式
     */
    template<typename T>
    typename McPrivate::SharedTypeSelector<T>::Type getBean(const QString &name,
                                                            QThread *thread = nullptr) noexcept
    {
        QVariant var = getBeanToVariant(name, thread);
        return var.value<typename McPrivate::SharedTypeSelector<T>::Type>();
    }
    /*!
     * \brief getBean
     * 
     * 根据bean的名称从容器中获取bean对象。
     * \note 此函数是线程安全的.
     * \param name beanName
     * \param thread 目标线程
     * \return 返回获取到的对象，可能为空
     */
    virtual QObjectPtr getBean(const QString &name, QThread *thread = nullptr) noexcept = 0;
    
    /*!
     * \brief getBeanToVariant
     * 
     * 根据bean的名称从容器中获取bean对象。
     * \note 此函数是线程安全的。
     * \warning 只有对象第一次生成的时候传入的Thread才有效，如果对象已经生成过，
     * 那么传入的Thread不会起任何作用
     * \param name beanName
     * \param thread 将要生存的目标线程，当前获取的bean和bean中的QObject属性都将
     * 存在于该线程，如果指定为null，则目标线程为调用该函数时的线程
     * \return 包含bean实例的QVariant，可能无效
     */
    virtual QVariant getBeanToVariant(const QString &name, QThread *thread = nullptr) noexcept = 0;
    
    /*!
     * \brief containsBean
     * 
     * 检测容器中是否存在该bean
     * \note 此函数是线程安全的。
     * \param name beanName
     * \return 是否存在
     * \retval true 存在
     * \retval false 不存在
     */
    virtual bool containsBean(const QString &name) noexcept = 0;
    /*!
     * \brief isSingleton
     * 
     * 检测bean是否为单例
     * \note 此函数是线程安全的。
     * \param name
     * \return 
     */
    virtual bool isSingleton(const QString &name) noexcept = 0;
};

MC_DECL_POINTER(IMcBeanFactory)
