#pragma once

#include "../McWidgetGlobal.h"

#include <QVariant>

class IMcWidgetBeanFactory
{
public:
    virtual ~IMcWidgetBeanFactory() = default;

    /*!
     * 此函数并不符合接口的设计，因为C++模板不能作用在纯虚函数上，
     * 提供此函数仅用于提供一种方便的向目标类型转换的方式
     */
    template<typename T>
    std::remove_pointer_t<T> *getBean(const QString &name, QWidget *parent = nullptr) const noexcept
    {
        auto w = getBean(name, parent);
        return qobject_cast<std::remove_pointer_t<T> *>(w);
    }

    /*!
     * \brief getBean
     * 注意：指定的父控件只会对获取的widget有效，如果该widget中包含其他控件，
     * 根据父子控件的关系，其他控件的会自动指定父控件为获取的控件，以此类推。
     * \param name
     * \param parent
     * \return 
     */
    virtual QWidget *getBean(const QString &name, QWidget *parent = nullptr) const noexcept = 0;

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
    virtual bool containsBean(const QString &name) const noexcept = 0;
};

MC_DECL_POINTER(IMcWidgetBeanFactory)
