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

#include "McAbstractBeanBuilder.h"

class IMcBeanBuildable;

MC_FORWARD_DECL_CLASS(McBeanConnector)
MC_FORWARD_DECL_CLASS(McBeanReference)

MC_FORWARD_DECL_PRIVATE_DATA(McObjectBeanBuilder)

class MC_IOC_EXPORT McObjectBeanBuilder : public McAbstractBeanBuilder
{
public:
    McObjectBeanBuilder() noexcept;
    ~McObjectBeanBuilder() override;

    void addConnector(const McBeanConnectorPtr &val) noexcept;
    void setParentBeanReference(const McBeanReferencePtr &val) noexcept;

protected:
    void complete(QVariant &bean, QThread *thread) noexcept override;

    virtual void setParent(QObject *bean, QObject *parent) noexcept;

    void addPropertyValue(QObject *bean, const QVariantMap &pros);
    void addObjectConnect(QObject *bean, const QVariantMap &pros);
    QObject *getPropertyObject(QObject *, const QString &proName, const QVariantMap &proValues) noexcept;

    void callStartFunction(QObject *bean, IMcBeanBuildable *buildableBean) noexcept;
    void callFinishedFunction(QObject *bean, IMcBeanBuildable *buildableBean) noexcept;
    void callThreadMovedFunction(QObject *bean, IMcBeanBuildable *buildableBean) noexcept;
    void callCompletedFunction(QObject *bean, IMcBeanBuildable *buildableBean, Qt::ConnectionType type) noexcept;
    void callTagFunction(QObject *bean, const char *tag, Qt::ConnectionType type = Qt::DirectConnection) noexcept;

private:
    MC_DECL_PRIVATE(McObjectBeanBuilder)
};
