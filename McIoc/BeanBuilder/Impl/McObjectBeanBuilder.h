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
    void doMoveToThread(const QVariant &bean, QThread *thread, const QVariantHash &properties) noexcept override;

    virtual void setParent(QObject *bean, QObject *parent) noexcept;

private:
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

MC_DECL_POINTER(McObjectBeanBuilder)
