#pragma once

#include <McIoc/McGlobal.h>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

class IMcBuiltInBeanBuilder
{
public:
    virtual ~IMcBuiltInBeanBuilder() = default;

    /*!
     * \brief build
     * 构造对象
     */
    virtual QWidget *build() noexcept = 0;
    /*!
     * \brief setProperty
     * 对已经构造出的对象设置参数
     * \note 调用此函数前会先调用build
     * \see build
     */
    virtual void setProperty(const QString &name, const QVariant &val) noexcept = 0;
};

MC_DECL_POINTER(IMcBuiltInBeanBuilder)
