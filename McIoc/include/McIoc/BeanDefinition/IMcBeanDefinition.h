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

#include "../McGlobal.h"

#include <QVariant>

class IMcBeanDefinition {
public:
    virtual ~IMcBeanDefinition() = default;

    virtual QVariant getBean() const noexcept = 0;
    virtual void setBean(const QVariant &bean) noexcept = 0;

    virtual bool isPointer() const noexcept = 0;
    virtual void setPointer(bool val) noexcept = 0;

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

MC_DECL_POINTER(IMcBeanDefinition)
