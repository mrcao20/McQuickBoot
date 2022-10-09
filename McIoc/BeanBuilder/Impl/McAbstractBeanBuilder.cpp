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
#include "McAbstractBeanBuilder.h"

#include <QMetaEnum>

#include "../IMcBeanReferenceResolver.h"
#include "../IMcCustomPlaceholder.h"
#include "McBeanEnum.h"
#include "McBeanPlaceholder.h"
#include "McBeanReference.h"

MC_DECL_PRIVATE_DATA(McAbstractBeanBuilder)
bool isSingleton{true};
McMetaType metaType;
QVariantHash properties;
QList<McAbstractBeanBuilder::ConstructorArg> constructorArgs;
IMcBeanReferenceResolver *resolver{nullptr};
QVariant bean;
QVariant earlyBean;
MC_DECL_PRIVATE_DATA_END

McAbstractBeanBuilder::McAbstractBeanBuilder() noexcept
{
    MC_NEW_PRIVATE_DATA(McAbstractBeanBuilder);
}

McAbstractBeanBuilder::~McAbstractBeanBuilder() {}

void McAbstractBeanBuilder::setSingleton(bool val) noexcept
{
    d->isSingleton = val;
}

McMetaType McAbstractBeanBuilder::metaType() const noexcept
{
    return d->metaType;
}

void McAbstractBeanBuilder::setMetaType(McMetaType type) noexcept
{
    d->metaType = type;
}

void McAbstractBeanBuilder::addProperty(const QString &name, const QVariant &value) noexcept
{
    d->properties.insert(name, value);
}

void McAbstractBeanBuilder::addConstructorArg(int index, const QVariant &val) noexcept
{
    d->constructorArgs.append({index, QByteArray(), val});
}

void McAbstractBeanBuilder::addConstructorArg(const QByteArray &name, const QVariant &val) noexcept
{
    d->constructorArgs.append({-1, name, val});
}

void McAbstractBeanBuilder::addConstructorArg(const ConstructorArg &arg) noexcept
{
    d->constructorArgs.append(arg);
}

IMcBeanReferenceResolver *McAbstractBeanBuilder::resolver() const noexcept
{
    return d->resolver;
}

QVariant McAbstractBeanBuilder::build(QThread *thread) noexcept
{
    if (d->earlyBean.isValid()) {
        return d->earlyBean;
    }
    if (d->bean.isValid()) {
        return d->bean;
    }
    auto bean = create();
    if (!bean.isValid()) {
        return QVariant();
    }
    d->earlyBean = bean;
    complete(bean, thread);
    if (isSingleton()) {
        d->bean = bean;
    }
    d->earlyBean.clear();
    return bean;
}

void McAbstractBeanBuilder::moveToThread(QThread *thread) noexcept
{
    if (Q_UNLIKELY(!d->bean.isValid())) {
        return;
    }
    doMoveToThread(d->bean, thread, d->properties);
}

bool McAbstractBeanBuilder::isSingleton() const noexcept
{
    return d->isSingleton;
}

void McAbstractBeanBuilder::setReferenceResolver(IMcBeanReferenceResolver *resolver) noexcept
{
    d->resolver = resolver;
}

QVariant McAbstractBeanBuilder::create() noexcept
{
    if (Q_UNLIKELY(!d->metaType.isValid())) {
        return QVariant();
    }
    if (hasConstructorArg()) {
        return createByMetaObject();
    } else {
        return createByMetaType();
    }
}

QVariant McAbstractBeanBuilder::convert(const QVariant &value, const QVariant &extra) const noexcept
{
    QVariant result;
#ifdef MC_USE_QT5
    auto type = value.userType();
    if (type == qMetaTypeId<McBeanReferencePtr>()) {
        result = convertRef(value, extra);
    } else if (type == qMetaTypeId<McBeanEnumPtr>()) {
        result = convertEnum(value, extra);
    } else if (type == qMetaTypeId<QVariantList>()) {
        result = convertList(value, extra);
    } else if (type == qMetaTypeId<QMap<QVariant, QVariant>>()) {
        result = convertMap(value, extra);
    } else {
        result = value;
    }
#else
    auto type = value.metaType();
    if (type == QMetaType::fromType<McBeanReferencePtr>()) {
        result = convertRef(value, extra);
    } else if (type == QMetaType::fromType<McBeanEnumPtr>()) {
        result = convertEnum(value, extra);
    } else if (type == QMetaType::fromType<QVariantList>()) {
        result = convertList(value, extra);
    } else if (type == QMetaType::fromType<QMap<QVariant, QVariant>>()) {
        result = convertMap(value, extra);
    } else {
        result = value;
    }
#endif
    return result;
}

QVariant McAbstractBeanBuilder::convertRef(const QVariant &value, const QVariant &extra) const noexcept
{
    Q_UNUSED(extra)
    auto ref = value.value<McBeanReferencePtr>();
    if (Q_UNLIKELY(ref.isNull() || d->resolver == nullptr)) {
        qCCritical(mcIoc(), "cannot inject beanReference");
        return QVariant();
    }
    QVariant objVar = d->resolver->resolveBeanReference(ref);
    if (Q_UNLIKELY(!objVar.isValid())) {
        qCCritical(mcIoc(), "cannot get bean from beanReference. for bean name: %s.", qPrintable(ref->name()));
        return QVariant();
    }
    return objVar;
}

QVariant McAbstractBeanBuilder::convertEnum(const QVariant &value, const QVariant &extra) const noexcept
{
    Q_UNUSED(extra)
    auto e = value.value<McBeanEnumPtr>();
    if (Q_UNLIKELY(e.isNull())) {
        qCCritical(mcIoc(), "cannot inject enum");
        return QVariant();
    }
    const QMetaObject *mo = nullptr;
#ifdef MC_USE_QT5
    if (e->scope() == "Qt") {
        mo = qt_getQtMetaObject();
    } else {
        int type = QMetaType::type(e->scope().toLocal8Bit());
        mo = QMetaType::metaObjectForType(type);
    }
#else
    if (e->scope() == "Qt") {
        mo = &Qt::staticMetaObject;
    } else {
        mo = QMetaType::fromName(e->scope().toLocal8Bit()).metaObject();
    }
#endif
    if (Q_UNLIKELY(mo == nullptr)) {
        qCCritical(mcIoc(), "cannot get meta object for type: %s\n", qPrintable(e->scope()));
        return QVariant();
    }
    auto enumIndex = mo->indexOfEnumerator(qPrintable(e->type()));
    if (Q_UNLIKELY(enumIndex == -1)) {
        qCCritical(mcIoc(), "not exists enum '%s' for type '%s'", qPrintable(e->type()), qPrintable(e->scope()));
        return QVariant();
    }
    auto me = mo->enumerator(enumIndex);
    return QVariant(me.keysToValue(qPrintable(e->value())));
}

QVariant McAbstractBeanBuilder::convertList(const QVariant &value, const QVariant &extra) const noexcept
{
    QVariantList result;

    auto list = value.value<QVariantList>();

    result.reserve(list.size());
    for (const auto &var : list) {
        result << convert(var, extra);
    }

    return result;
}

QVariant McAbstractBeanBuilder::convertMap(const QVariant &value, const QVariant &extra) const noexcept
{
    QMap<QVariant, QVariant> result;

    auto map = value.value<QMap<QVariant, QVariant>>();

    QMapIterator<QVariant, QVariant> itr(map);
    while (itr.hasNext()) {
        auto item = itr.next();
        auto key = item.key();
        auto value = item.value();

        QVariant resultKey, resultValue;

        resultValue = convert(value, extra);
#ifdef MC_USE_QT5
        if (key.userType() == qMetaTypeId<McBeanPlaceholderPtr>()) {
#else
        if (key.metaType() == QMetaType::fromType<McBeanPlaceholderPtr>()) {
#endif
            auto valueObj = resultValue.value<QObject *>();
            auto keyPlh = key.value<McBeanPlaceholderPtr>();
            auto plh = keyPlh->placeholder();
            if (plh == "className") {
                resultKey = valueObj == nullptr ? "" : valueObj->metaObject()->className();
            } else if (plh == "objectName") {
                resultKey = valueObj == nullptr ? "" : valueObj->objectName();
            } else if (plh == "custom") {
                IMcCustomPlaceholder *customPlh = nullptr;
                QObject *plhObj = resultValue.value<QObject *>();
                if (plhObj != nullptr) {
                    customPlh = static_cast<IMcCustomPlaceholder *>(plhObj->qt_metacast("IMcCustomPlaceholder"));
                }
                if (customPlh == nullptr) {
                    customPlh = resultValue.value<IMcCustomPlaceholder *>();
                }
                resultKey = customPlh == nullptr ? "" : customPlh->getKey();
            } else {
                qCCritical(mcIoc(), "the plh must be one of className/objectName/custom");
            }
        } else {
            resultKey = convert(key, extra);
        }

        if (resultKey.isValid() && resultValue.isValid()) {
            result.insert(resultKey, resultValue);
        }
    }

    return QVariant::fromValue(result);
}

bool McAbstractBeanBuilder::hasConstructorArg() const noexcept
{
    return !d->constructorArgs.isEmpty();
}

QVariantMap McAbstractBeanBuilder::buildProperties(const QVariant &extra) const noexcept
{
    QVariantMap pros;
    QHashIterator<QString, QVariant> itr(d->properties);
    while (itr.hasNext()) {
        auto item = itr.next();
        pros.insert(item.key(), convert(item.value(), extra));
    }
    return pros;
}

QVariant McAbstractBeanBuilder::createByMetaType() noexcept
{
#ifdef MC_USE_QT5
    auto beanStar = d->metaType.createPointer();
#else
    auto beanStar = d->metaType.metaType().create();
#endif
    if (Q_UNLIKELY(beanStar == nullptr)) {
#ifdef MC_USE_QT5
        qCCritical(mcIoc(), "cannot create object: '%s'", QMetaType::typeName(d->metaType.pMetaType()));
#else
        qCCritical(mcIoc(), "cannot create object: '%s'", d->metaType.metaType().name());
#endif
        return QVariant();
    }
    QVariant beanVar(d->metaType.pMetaType(), &beanStar);
    return beanVar;
}

QVariant McAbstractBeanBuilder::createByMetaObject() noexcept
{
    if (d->constructorArgs.size() > Q_METAMETHOD_INVOKE_MAX_ARGS) {
        qCCritical(mcIoc(), "the number of constructor parameters cannot more than %d", Q_METAMETHOD_INVOKE_MAX_ARGS);
        return QVariant();
    }
#ifdef MC_USE_QT5
    auto metaObj = QMetaType::metaObjectForType(d->metaType.pMetaType());
#else
    auto metaObj = d->metaType.pMetaType().metaObject();
#endif
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
#ifdef MC_USE_QT5
        QByteArray typeName = QMetaType::typeName(constructor.parameterType(i));
#else
        QByteArray typeName = constructor.parameterTypeName(i);
#endif
        arguments.replace(i, QGenericArgument(typeName.constData(), argValues[i].constData()));
    }
    void *beanStar = nullptr;
    void *param[] = {&beanStar, arguments.at(0).data(), arguments.at(1).data(), arguments.at(2).data(),
        arguments.at(3).data(), arguments.at(4).data(), arguments.at(5).data(), arguments.at(6).data(),
        arguments.at(7).data(), arguments.at(8).data(), arguments.at(9).data()};
    auto idx = metaObj->indexOfConstructor(constructor.methodSignature().constData());
    if (metaObj->static_metacall(QMetaObject::CreateInstance, idx, param) >= 0 || beanStar == nullptr) {
#ifdef MC_USE_QT5
        qCCritical(mcIoc(), "cannot create object: '%s'", QMetaType::typeName(d->metaType.pMetaType()));
#else
        qCCritical(mcIoc(), "cannot create object: '%s'", d->metaType.metaType().name());
#endif
        return QVariant();
    }
    QVariant beanVar(d->metaType.pMetaType(), &beanStar);
    return beanVar;
}
