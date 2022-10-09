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
#include "McGadgetBeanBuilder.h"

#include <QMetaMethod>

#include "Utils/IMcBeanBuildable.h"

void McGadgetBeanBuilder::setClassName(const QByteArray &className) noexcept
{
#ifdef MC_USE_QT5
    McMetaType type = McMetaType::fromPTypeName(className + "*");
#else
    McMetaType type = McMetaType::fromTypeName(className);
#endif
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
