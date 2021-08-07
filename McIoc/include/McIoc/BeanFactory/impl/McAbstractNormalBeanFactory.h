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

#include "McAbstractBeanFactory.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractNormalBeanFactory);

class MCIOC_EXPORT McAbstractNormalBeanFactory : public McAbstractBeanFactory
{
    Q_OBJECT
public:
    McAbstractNormalBeanFactory(QObject *parent = nullptr);
    ~McAbstractNormalBeanFactory() override;

    IMcPropertyConverterPtr getPropertyConverter() const noexcept override;
    void setPropertyConverter(IMcPropertyConverterConstPtrRef converter) noexcept override;

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

protected:
    virtual QVariant convertToQVariant(QObject *obj) noexcept = 0;
    virtual QVariant convertToQVariant(void *gadget, const QMetaObject *metaObj) noexcept = 0;

private:
    /*!
     * \brief callTagFunction
     * 
     * 调用被某一个tag声明的函数
     * \param bean
     * \param tag
     */
    void callTagFunction(QObject *bean,
                         const char *tag,
                         Qt::ConnectionType type = Qt::DirectConnection) noexcept;
    void callTagFunction(void *bean, const QMetaObject *metaObj, const char *tag) noexcept;
    /*!
     * \brief callStartFunction
     * 
     * 调用构造开始函数
     * \param bean
     */
    void callStartFunction(QObject *bean) noexcept;
    void callStartFunction(void *bean, const QMetaObject *metaObj) noexcept;
    QVariant parseOnGadget(IMcBeanDefinitionConstPtrRef beanDefinition) noexcept;
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
    bool addPropertyValue(QObject *bean,
                          IMcBeanDefinitionConstPtrRef beanDefinition,
                          QVariantMap &proValues) noexcept;
    bool addPropertyValue(void *bean,
                          const QMetaObject *metaObj,
                          IMcBeanDefinitionConstPtrRef beanDefinition) noexcept;

    /*!
     * \brief addObjectConnect
     * 
     * 为bean添加对应信号槽
     * \param bean bean实例
     * \param beanDefinition bean定义
     * \param proValues bean中的属性
     * \return 是否全部成功
     */
    bool addObjectConnect(QObject *bean,
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
    QObject *getPropertyObject(QObject *,
                               const QString &proName,
                               const QVariantMap &proValues) noexcept;

    /*!
     * \brief callFinishedFunction
     * 
     * 调用构造完成函数
     * \param bean
     */
    void callFinishedFunction(QObject *) noexcept;
    void callFinishedFunction(void *bean, const QMetaObject *metaObj) noexcept;
    /*!
     * \brief callThreadFinishedFunction
     * 
     * 调用线程移动结束函数，如果线程未被移动，则不会调用
     * \param bean
     */
    void callThreadFinishedFunction(QObject *bean) noexcept;

private:
    MC_DECL_PRIVATE(McAbstractNormalBeanFactory)
};

MC_DECL_POINTER(McAbstractNormalBeanFactory)
