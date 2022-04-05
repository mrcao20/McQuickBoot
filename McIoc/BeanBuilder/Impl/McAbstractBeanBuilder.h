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

#include "../IMcBeanBuilder.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractBeanBuilder)

class MC_IOC_EXPORT McAbstractBeanBuilder : public IMcBeanBuilder
{
public:
    McAbstractBeanBuilder() noexcept;
    ~McAbstractBeanBuilder() override;

    void setSingleton(bool val) noexcept;
    void addProperty(const QString &name, const QVariant &value) noexcept;
    IMcBeanReferenceResolver *resolver() const noexcept;

    QVariant build(QThread *thread) noexcept override;
    void moveToThread(QThread *thread) noexcept override;
    bool isSingleton() const noexcept override;
    void setReferenceResolver(IMcBeanReferenceResolver *resolver) noexcept override;

protected:
    virtual QVariant create() noexcept = 0;
    virtual void complete(QVariant &bean, QThread *thread) noexcept = 0;
    virtual void doMoveToThread(const QVariant &bean, QThread *thread, const QVariantHash &properties) noexcept = 0;

    virtual QVariant convert(const QVariant &value, const QVariant &extra) const noexcept;
    virtual QVariant convertRef(const QVariant &value, const QVariant &extra) const noexcept;
    virtual QVariant convertEnum(const QVariant &value, const QVariant &extra) const noexcept;
    virtual QVariant convertList(const QVariant &value, const QVariant &extra) const noexcept;
    virtual QVariant convertMap(const QVariant &value, const QVariant &extra) const noexcept;

    QVariantMap buildProperties(const QVariant &extra = QVariant()) const noexcept;

private:
    MC_DECL_PRIVATE(McAbstractBeanBuilder)
};

MC_DECL_POINTER(McAbstractBeanBuilder)
