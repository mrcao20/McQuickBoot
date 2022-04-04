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

#include "../IMcCustomPlaceholder.h"
#include "BeanFactory/IMcBeanReferenceResolver.h"
#include "McBeanEnum.h"
#include "McBeanPlaceholder.h"
#include "McBeanReference.h"

MC_DECL_PRIVATE_DATA(McAbstractBeanBuilder)
bool isSingleton{true};
QVariantHash properties;
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

void McAbstractBeanBuilder::addProperty(const QString &name, const QVariant &value) noexcept
{
    d->properties.insert(name, value);
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
    d->earlyBean = bean;
    complete(bean, thread);
    if (isSingleton()) {
        d->bean = bean;
    }
    d->earlyBean.clear();
    return bean;
}

bool McAbstractBeanBuilder::isSingleton() const noexcept
{
    return d->isSingleton;
}

void McAbstractBeanBuilder::setReferenceResolver(IMcBeanReferenceResolver *resolver) noexcept
{
    d->resolver = resolver;
}

QVariant McAbstractBeanBuilder::convert(const QVariant &value, const QVariant &extra) const noexcept
{
    QVariant result;
    if (value.canConvert<McBeanReferencePtr>()) {
        result = convertRef(value, extra);
    } else if (value.canConvert<McBeanEnumPtr>()) {
        result = convertEnum(value, extra);
    } else if (value.canConvert<QVariantList>()) {
        result = convertList(value, extra);
    } else if (value.canConvert<QMap<QVariant, QVariant>>()) {
        result = convertMap(value, extra);
    } else {
        result = value;
    }
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
    QVariant objVar = d->resolver->resolveBeanReferenceToQVariant(ref);
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
    if (e->scope() == "Qt") {
        mo = &Qt::staticMetaObject;
    } else {
        mo = QMetaType::fromName(e->scope().toLocal8Bit()).metaObject();
    }
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
        if (key.metaType() == QMetaType::fromType<McBeanPlaceholderPtr>()) {
            auto valueObj = resultValue.value<QObject *>();
            auto keyPlh = key.value<McBeanPlaceholderPtr>();
            auto plh = keyPlh->placeholder();
            if (plh == "className") {
                resultKey = valueObj == nullptr ? "" : valueObj->metaObject()->className();
            } else if (plh == "objectName") {
                resultKey = valueObj == nullptr ? "" : valueObj->objectName();
            } else if (plh == "custom") {
                auto customPlh = resultValue.value<IMcCustomPlaceholder *>();
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
