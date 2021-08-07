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

#include <McIoc/BeanDefinition/IMcBeanDefinition.h>

class McSimpleBeanDefinition : public QObject, public IMcBeanDefinition
{
    Q_OBJECT
public:
    QVariant getBean() const noexcept override { return m_bean; }
    void setBean(const QVariant &bean) noexcept override { m_bean = bean; }

    bool isPointer() const noexcept override { return false; }
    void setPointer(bool val) noexcept override { Q_UNUSED(val) }

    bool isSingleton() const noexcept override { return true; }
    void setSingleton(bool val) noexcept override { Q_UNUSED(val) }

    const QMetaObject *getBeanMetaObject() const noexcept override { return nullptr; }
    void setBeanMetaObject(const QMetaObject *o) noexcept override { Q_UNUSED(o) }

    QString getClassName() const noexcept override { return ""; }
    void setClassName(const QString &name) noexcept override { Q_UNUSED(name) }

    QString getPluginPath() const noexcept override { return ""; }
    void setPluginPath(const QString &path) noexcept override { Q_UNUSED(path) }

    QVariantHash getProperties() const noexcept override { return QVariantHash(); }
    void addProperty(const QString &name, const QVariant &value) noexcept override
    {
        Q_UNUSED(name)
        Q_UNUSED(value)
    }

    QVariantList getConnectors() const noexcept override { return QVariantList(); }
    void addConnector(const QVariant &val) noexcept override { Q_UNUSED(val) }

private:
    QVariant m_bean;
};

MC_DECL_POINTER(McSimpleBeanDefinition)
