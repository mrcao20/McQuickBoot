/*
 * Copyright (c) 2021 mrcao20/mrcao20@163.com
 * McQuickBoot is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#pragma once

#include "../IMcBeanBuilder.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractBeanBuilder)

class MC_IOC_EXPORT McAbstractBeanBuilder : public IMcBeanBuilder
{
public:
    struct ConstructorArg
    {
        int index{-1};
        QByteArray name;
        QVariant value;
    };
    McAbstractBeanBuilder() noexcept;
    ~McAbstractBeanBuilder() override;

    void setSingleton(bool val) noexcept;
    McMetaType metaType() const noexcept;
    void setMetaType(McMetaType type) noexcept;
    void addProperty(const QString &name, const QVariant &value) noexcept;
    void addConstructorArg(int index, const QVariant &val) noexcept;
    void addConstructorArg(const QByteArray &name, const QVariant &val) noexcept;
    void addConstructorArg(const ConstructorArg &arg) noexcept;
    IMcBeanReferenceResolver *resolver() const noexcept;

    QVariant build(QThread *thread) noexcept override final;
    void moveToThread(QThread *thread) noexcept override final;
    bool isSingleton() const noexcept override;
    const QMetaObject *getBeanMetaObject() const noexcept override;
    void setReferenceResolver(IMcBeanReferenceResolver *resolver) noexcept override final;

protected:
    virtual void complete(QVariant &bean, QThread *thread) noexcept = 0;
    virtual void doMoveToThread(const QVariant &bean, QThread *thread, const QVariantHash &properties) noexcept = 0;

    virtual QVariant create() noexcept;

    virtual QVariant convert(const QVariant &value, const QVariant &extra) const noexcept;
    virtual QVariant convertRef(const QVariant &value, const QVariant &extra) const noexcept;
    virtual QVariant convertEnum(const QVariant &value, const QVariant &extra) const noexcept;
    virtual QVariant convertList(const QVariant &value, const QVariant &extra) const noexcept;
    virtual QVariant convertMap(const QVariant &value, const QVariant &extra) const noexcept;

    bool hasConstructorArg() const noexcept;
    QVariantMap buildProperties(const QVariant &extra = QVariant()) const noexcept;

private:
    QVariant createByMetaType() noexcept;
    QVariant createByMetaObject() noexcept;

private:
    MC_DECL_PRIVATE(McAbstractBeanBuilder)
};

MC_DECL_POINTER(McAbstractBeanBuilder)
