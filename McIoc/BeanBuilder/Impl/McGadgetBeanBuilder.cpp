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

MC_DECL_PRIVATE_DATA(McGadgetBeanBuilder)
McMetaType metaType;
QList<McGadgetBeanBuilder::ConstructorArg> constructorArgs;
MC_DECL_PRIVATE_DATA_END

McGadgetBeanBuilder::McGadgetBeanBuilder() noexcept
{
    MC_NEW_PRIVATE_DATA(McGadgetBeanBuilder);
}

McGadgetBeanBuilder::~McGadgetBeanBuilder() {}

McMetaType McGadgetBeanBuilder::metaType() const noexcept
{
    return d->metaType;
}

void McGadgetBeanBuilder::setMetaType(const McMetaType &type) noexcept
{
    d->metaType = type;
}

void McGadgetBeanBuilder::setClassName(const QByteArray &className) noexcept
{
    McMetaType type = McMetaType::fromTypeName(className);
    if (Q_UNLIKELY(!type.isValid())) {
        qCCritical(mcIoc(), "class '%s' is not registered", className.constData());
        return;
    }
    setMetaType(type);
}

void McGadgetBeanBuilder::addConstructorArg(int index, const QVariant &val) noexcept
{
    d->constructorArgs.append({index, QByteArray(), val});
}

void McGadgetBeanBuilder::addConstructorArg(const QByteArray &name, const QVariant &val) noexcept
{
    d->constructorArgs.append({-1, name, val});
}

bool McGadgetBeanBuilder::isPointer() const noexcept
{
    return true;
}

QVariant McGadgetBeanBuilder::create() noexcept
{
    if (Q_UNLIKELY(!d->metaType.isValid())) {
        return QVariant();
    }
    if (d->constructorArgs.isEmpty()) {
        return createByMetaType();
    } else {
        return createByMetaObject();
    }
}

void McGadgetBeanBuilder::complete(QVariant &bean, QThread *thread) noexcept
{
    Q_UNUSED(thread)
    if (Q_UNLIKELY(!d->metaType.isValid())) {
        return;
    }
    auto metaObject = d->metaType.metaType().metaObject();
    auto beanStar = bean.data();
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
        auto key = itr.key();
        auto value = itr.value();

        auto index = metaObject->indexOfProperty(key.toLocal8Bit());
        if (index == -1) {
            qCDebug(mcIoc(),
                    "bean '%s' cannot found property named for '%s'.",
                    metaObject->className(),
                    qPrintable(key));

        } else {
            auto metaProperty = metaObject->property(index);
            if (Q_UNLIKELY(!metaProperty.writeOnGadget(bean, value))) {
                qCCritical(mcIoc(),
                           "bean '%s' write property named for '%s' failure",
                           metaObject->className(),
                           qPrintable(key));
            }
        }
    }
}

void McGadgetBeanBuilder::callStartFunction(void *bean,
                                            const QMetaObject *metaObject,
                                            IMcBeanBuildable *buildableBean) noexcept
{
    callTagFunction(bean, metaObject, MC_STRINGIFY(MC_STARTED));
    if (buildableBean != nullptr) {
        buildableBean->buildStarted();
    }
}

void McGadgetBeanBuilder::callFinishedFunction(void *bean,
                                               const QMetaObject *metaObject,
                                               IMcBeanBuildable *buildableBean) noexcept
{
    callTagFunction(bean, metaObject, MC_STRINGIFY(MC_FINISHED));
    if (buildableBean != nullptr) {
        buildableBean->buildFinished();
    }
}

void McGadgetBeanBuilder::callTagFunction(void *bean, const QMetaObject *metaObject, const char *tag) noexcept
{
    if (metaObject == nullptr) {
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

QVariant McGadgetBeanBuilder::createByMetaType() noexcept
{
    auto beanStar = d->metaType.metaType().create();
    if (Q_UNLIKELY(beanStar == nullptr)) {
        qCCritical(mcIoc(), "cannot create object: '%s'", d->metaType.metaType().name());
        return QVariant();
    }
    QVariant beanVar(d->metaType.pMetaType(), &beanStar);
    return beanVar;
}

QVariant McGadgetBeanBuilder::createByMetaObject() noexcept
{
    if (d->constructorArgs.size() > Q_METAMETHOD_INVOKE_MAX_ARGS) {
        qCCritical(mcIoc(), "the number of constructor parameters cannot more than %d", Q_METAMETHOD_INVOKE_MAX_ARGS);
        return QVariant();
    }
    auto metaObj = d->metaType.pMetaType().metaObject();
    if (Q_UNLIKELY(metaObj == nullptr)) {
        return QVariant();
    }
    QMap<int, QVariant> argValues;
    QMetaMethod constructor;
    for (int i = 0; i < metaObj->constructorCount(); ++i) {
        auto method = metaObj->constructor(i);
        if (method.parameterCount() != d->constructorArgs.size()) {
            continue;
        }
        bool isOk = true;
        auto paramNames = method.parameterNames();
        for (auto &arg : qAsConst(d->constructorArgs)) {
            int index = -1;
            if (arg.index >= 0 && arg.index < method.parameterCount()) {
                index = arg.index;
            } else {
                index = paramNames.indexOf(arg.name);
            }
            if (index == -1) {
                isOk = false;
                break;
            }
            if (argValues.contains(index)) {
                isOk = false;
                break;
            }
            argValues.insert(index, convert(arg.value, QVariant()));
        }
        if (isOk) {
            constructor = method;
            break;
        } else {
            argValues.clear();
        }
    }
    if (argValues.size() != d->constructorArgs.size()) {
        return QVariant();
    }
    QVector<QGenericArgument> arguments(Q_METAMETHOD_INVOKE_MAX_ARGS);
    for (int i = 0; i < argValues.size(); ++i) {
        arguments.replace(i, QGenericArgument(constructor.parameterTypeName(i).constData(), argValues[i].constData()));
    }
    void *beanStar = nullptr;
    void *param[] = {&beanStar,
                     arguments.at(0).data(),
                     arguments.at(1).data(),
                     arguments.at(2).data(),
                     arguments.at(3).data(),
                     arguments.at(4).data(),
                     arguments.at(5).data(),
                     arguments.at(6).data(),
                     arguments.at(7).data(),
                     arguments.at(8).data(),
                     arguments.at(9).data()};
    auto idx = metaObj->indexOfConstructor(constructor.methodSignature().constData());
    if (metaObj->static_metacall(QMetaObject::CreateInstance, idx, param) < 0 || beanStar == nullptr) {
        qCCritical(mcIoc(), "cannot create object: '%s'", d->metaType.metaType().name());
        return QVariant();
    }
    QVariant beanVar(d->metaType.pMetaType(), &beanStar);
    return beanVar;
}
