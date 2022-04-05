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
#include "McObjectClassBeanBuilder.h"

#include <QMetaMethod>

MC_DECL_PRIVATE_DATA(McObjectClassBeanBuilder)
McMetaType metaType;
QList<McObjectClassBeanBuilder::ConstructorArg> constructorArgs;
MC_DECL_PRIVATE_DATA_END

McObjectClassBeanBuilder::McObjectClassBeanBuilder() noexcept
{
    MC_NEW_PRIVATE_DATA(McObjectClassBeanBuilder);
}

McObjectClassBeanBuilder::~McObjectClassBeanBuilder() {}

McMetaType McObjectClassBeanBuilder::metaType() const noexcept
{
    return d->metaType;
}

void McObjectClassBeanBuilder::setMetaType(const McMetaType &type) noexcept
{
    d->metaType = type;
}

void McObjectClassBeanBuilder::setClassName(const QByteArray &className) noexcept
{
    McMetaType type = McMetaType::fromTypeName(className);
    if (Q_UNLIKELY(!type.isValid())) {
        qCCritical(mcIoc(), "class '%s' is not registered", className.constData());
        return;
    }
    setMetaType(type);
}

void McObjectClassBeanBuilder::addConstructorArg(int index, const QVariant &val) noexcept
{
    d->constructorArgs.append({index, QByteArray(), val});
}

void McObjectClassBeanBuilder::addConstructorArg(const QByteArray &name, const QVariant &val) noexcept
{
    d->constructorArgs.append({-1, name, val});
}

bool McObjectClassBeanBuilder::isPointer() const noexcept
{
    return true;
}

QVariant McObjectClassBeanBuilder::create() noexcept
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

QVariant McObjectClassBeanBuilder::createByMetaType() noexcept
{
    auto beanStar = d->metaType.metaType().create();
    if (Q_UNLIKELY(beanStar == nullptr)) {
        qCCritical(mcIoc(), "cannot create object: '%s'", d->metaType.metaType().name());
        return QVariant();
    }
    QVariant beanVar(d->metaType.pMetaType(), &beanStar);
    return beanVar;
}

QVariant McObjectClassBeanBuilder::createByMetaObject() noexcept
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
    auto obj = metaObj->newInstance(arguments.at(0),
                                    arguments.at(1),
                                    arguments.at(2),
                                    arguments.at(3),
                                    arguments.at(4),
                                    arguments.at(5),
                                    arguments.at(6),
                                    arguments.at(7),
                                    arguments.at(8),
                                    arguments.at(9));
    if (obj == nullptr) {
        qCCritical(mcIoc(), "cannot create object: '%s'", d->metaType.metaType().name());
        return QVariant();
    }
    QVariant beanVar = QVariant::fromValue(obj);
    if (!beanVar.convert(d->metaType.pMetaType())) {
        return QVariant();
    }
    return beanVar;
}
