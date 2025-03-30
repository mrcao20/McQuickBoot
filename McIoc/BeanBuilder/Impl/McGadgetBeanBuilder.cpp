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
#include "McGadgetBeanBuilder.h"

#include <QMetaMethod>

#include "Utils/IMcBeanBuildable.h"

void McGadgetBeanBuilder::setClassName(const QByteArray &className) noexcept
{
    McMetaType type = McMetaType::fromTypeName(className);
    if (Q_UNLIKELY(!type.isValid())) {
        qCCritical(mcIoc(), "class '%s' is not registered", className.constData());
        return;
    }
    setMetaType(type);
}

bool McGadgetBeanBuilder::isPointer() const noexcept
{
    return true;
}

void McGadgetBeanBuilder::complete(QVariant &bean, QThread *thread) noexcept
{
    Q_UNUSED(thread)
    if (Q_UNLIKELY(!metaType().isValid())) {
        return;
    }
#ifdef MC_USE_QT5
    auto metaObject = QMetaType::metaObjectForType(metaType().pMetaType());
#else
    auto metaObject = metaType().metaType().metaObject();
#endif
    auto beanStar = *reinterpret_cast<void **>(bean.data());
    auto buildableObj = bean.value<IMcBeanBuildable *>();
    auto properties = buildProperties();
    callStartFunction(beanStar, metaObject, buildableObj);
    addPropertyValue(beanStar, metaObject, properties);
    callFinishedFunction(beanStar, metaObject, buildableObj);
}

void McGadgetBeanBuilder::doMoveToThread(const QVariant &bean, QThread *thread, const QVariantHash &properties) noexcept
{
    Q_UNUSED(bean)
    Q_UNUSED(thread)
    Q_UNUSED(properties)
}

void McGadgetBeanBuilder::addPropertyValue(void *bean, const QMetaObject *metaObject, const QVariantMap &pros)
{
    if (bean == nullptr || metaObject == nullptr) {
        return;
    }
    QMapIterator<QString, QVariant> itr(pros);
    while (itr.hasNext()) {
        auto item = itr.next();
        auto key = item.key();
        auto value = item.value();

        auto index = metaObject->indexOfProperty(key.toLocal8Bit());
        if (index == -1) {
            qCDebug(
                mcIoc(), "bean '%s' cannot found property named for '%s'.", metaObject->className(), qPrintable(key));

        } else {
            auto metaProperty = metaObject->property(index);
            if (Q_UNLIKELY(!metaProperty.writeOnGadget(bean, value))) {
                qCCritical(mcIoc(), "bean '%s' write property named for '%s' failure", metaObject->className(),
                    qPrintable(key));
            }
        }
    }
}

void McGadgetBeanBuilder::callStartFunction(
    void *bean, const QMetaObject *metaObject, IMcBeanBuildable *buildableBean) noexcept
{
    callTagFunction(bean, metaObject, MC_STRINGIFY(MC_STARTED));
    if (buildableBean != nullptr) {
        buildableBean->buildStarted();
    }
}

void McGadgetBeanBuilder::callFinishedFunction(
    void *bean, const QMetaObject *metaObject, IMcBeanBuildable *buildableBean) noexcept
{
    callTagFunction(bean, metaObject, MC_STRINGIFY(MC_FINISHED));
    if (buildableBean != nullptr) {
        buildableBean->buildFinished();
    }
}

void McGadgetBeanBuilder::callTagFunction(void *bean, const QMetaObject *metaObject, const char *tag) noexcept
{
    if (bean == nullptr || metaObject == nullptr) {
        return;
    }
    for (int i = 0; i < metaObject->methodCount(); ++i) {
        auto method = metaObject->method(i);
        if (!Mc::isContainedTag(method.tag(), tag)) {
            continue;
        }
        //! 遍历当前对象的所有方法，调用所有被标记过的方法，从超基类开始到子类
        method.invokeOnGadget(bean);
    }
}
