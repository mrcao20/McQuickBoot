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
#include "McAnnotationBeanDefinitionReader.h"

#include <QMetaClassInfo>

#include "BeanBuilder/Impl/McBeanConnector.h"
#include "BeanBuilder/Impl/McBeanReference.h"
#include "BeanBuilder/Impl/McSharedGadgetBeanBuilder.h"
#include "BeanBuilder/Impl/McSharedObjectClassBeanBuilder.h"
#include "BeanFactory/IMcBeanBuilderRegistry.h"

MC_DECL_PRIVATE_DATA(McAnnotationBeanDefinitionReader)
QVector<McMetaType> metaTypes;
QMultiMap<QString, McBeanConnectorPtr> beanConnectors;
QHash<McMetaType, QSet<QString>> metaTypeBeanNames;
MC_DECL_PRIVATE_DATA_END

McAnnotationBeanDefinitionReader::McAnnotationBeanDefinitionReader(
    const QVector<McMetaType> &metaTypes, const QMultiMap<QString, McBeanConnectorPtr> &beanConnectors) noexcept
{
    MC_NEW_PRIVATE_DATA(McAnnotationBeanDefinitionReader);

    d->metaTypes = metaTypes;
    d->beanConnectors = beanConnectors;
}

McAnnotationBeanDefinitionReader::~McAnnotationBeanDefinitionReader() {}

void McAnnotationBeanDefinitionReader::doReadBeanBuilder() noexcept
{
    auto components = parseComponent();
    QMapIterator<QString, McMetaType> itr(components);
    while (itr.hasNext()) {
        auto item = itr.next();
        auto beanName = item.key();
        auto metaType = item.value();
        if (registry()->isContained(beanName)) {
            continue;
        }
        auto metaObj = metaType.pMetaType().metaObject();
        Q_ASSERT(metaObj != nullptr);
        bool isSingleton = true; //!< 默认为单例
        auto singletonIndex = metaObj->indexOfClassInfo(MC_SINGLETON_TAG);
        if (singletonIndex != -1) {
            auto classInfo = metaObj->classInfo(singletonIndex);
            isSingleton = classInfo.value() != QLatin1String("false");
        }
        bool isPointer = false; //!< 默认不为原始指针
        auto pointerIndex = metaObj->indexOfClassInfo(MC_POINTER_TAG);
        if (pointerIndex != -1) {
            auto classInfo = metaObj->classInfo(pointerIndex);
            isPointer = classInfo.value() == QLatin1String("true");
        }
        McAbstractBeanBuilderPtr builder;
        if (metaType.pMetaType().flags().testFlag(QMetaType::PointerToQObject)) {
            McObjectClassBeanBuilderPtr objectBuilder;
            if (isPointer) {
                objectBuilder = McObjectClassBeanBuilderPtr::create();
            } else {
                objectBuilder = McSharedObjectClassBeanBuilderPtr::create();
            }
            auto connectors = d->beanConnectors.values(beanName);
            for (auto &con : qAsConst(connectors)) {
                objectBuilder->addConnector(con);
            }
            builder = objectBuilder;
        } else {
            McGadgetBeanBuilderPtr gadgetBuilder;
            if (isPointer) {
                gadgetBuilder = McGadgetBeanBuilderPtr::create();
            } else {
                gadgetBuilder = McSharedGadgetBeanBuilderPtr::create();
            }
            builder = gadgetBuilder;
        }
        parseProperty(builder, metaObj);
        builder->setMetaType(metaType);
        builder->setSingleton(isSingleton);
        registry()->registerBeanBuilder(beanName, builder);
    }
}

void McAnnotationBeanDefinitionReader::addMetaTypeBeanName(McMetaType type, const QString &beanName) noexcept
{
    addMetaTypeBeanName(QVector<McMetaType>{type}, beanName);
}

void McAnnotationBeanDefinitionReader::addMetaTypeBeanName(
    const QVector<McMetaType> &types, const QString &beanName) noexcept
{
    for (auto &type : types) {
        d->metaTypeBeanNames[type].insert(beanName);
    }
}

QSet<QString> McAnnotationBeanDefinitionReader::getBeanNameForMetaType(McMetaType type) noexcept
{
    return d->metaTypeBeanNames.value(type);
}

QMap<QString, McMetaType> McAnnotationBeanDefinitionReader::parseComponent() noexcept
{
    QMap<QString, McMetaType> components;
    for (auto &metaType : qAsConst(d->metaTypes)) {
        auto metaObj = metaType.pMetaType().metaObject();
        if (metaObj == nullptr) {
            continue;
        }
        auto componentIndex = metaObj->indexOfClassInfo(MC_COMPONENT_TAG);
        if (componentIndex == -1) {
            continue;
        }
        QString beanName = McPrivate::getBeanName(metaObj);
        addMetaTypeBeanName(metaType, beanName);
        addMetaTypeBeanName(metaType.parentMetaTypes(), beanName);
        components.insert(beanName, metaType);
    }
    return components;
}

void McAnnotationBeanDefinitionReader::parseProperty(
    const McAbstractBeanBuilderPtr &builder, const QMetaObject *metaObj) noexcept
{
    if (builder.isNull() || metaObj == nullptr) {
        return;
    }
    for (int i = 0, count = metaObj->classInfoCount(); i < count; ++i) {
        auto classInfo = metaObj->classInfo(i);
        if (qstrcmp(classInfo.name(), MC_AUTOWIRED_TAG) == 0) {
            parsePropertyOnAutowired(builder, metaObj, classInfo.value());
        } else if (qstrcmp(classInfo.name(), MC_RESOURCE_TAG) == 0) {
            parsePropertyOnResource(builder, metaObj, classInfo.value());
        }
    }
}

void McAnnotationBeanDefinitionReader::parsePropertyOnAutowired(
    const McAbstractBeanBuilderPtr &builder, const QMetaObject *metaObj, const QString &value) noexcept
{
    if (builder.isNull() || metaObj == nullptr) {
        return;
    }
    auto list = value.split('=', Qt::SkipEmptyParts);
    if (list.isEmpty()) {
        return;
    }
    QByteArray proName = list.constFirst().simplified().toLatin1();
    QByteArray beanName = list.constLast().simplified().toLatin1();
    if (metaObj->indexOfProperty(proName.constData()) == -1) {
        return;
    }
    McBeanReferencePtr beanRef = McBeanReferencePtr::create();
    beanRef->setName(beanName);
    builder->addProperty(proName, QVariant::fromValue(beanRef));
}

void McAnnotationBeanDefinitionReader::parsePropertyOnResource(
    const McAbstractBeanBuilderPtr &builder, const QMetaObject *metaObj, const QByteArray &value) noexcept
{
    if (builder.isNull() || metaObj == nullptr) {
        return;
    }
    auto proName = value;
    auto proIndex = metaObj->indexOfProperty(proName);
    if (proIndex == -1) {
        qCCritical(mcIoc(), "Cannot found property: %s. for class: %s", proName.constData(), metaObj->className());
        return;
    }
    auto pro = metaObj->property(proIndex);
    auto qmetaType = pro.metaType();
    auto listMetaType = McListMetaType::fromQMetaType(qmetaType);
    if (listMetaType.isValid()) {
        auto metaType = getMcMetaType(listMetaType.valueMetaType());
        if (!metaType.isValid()) {
            return;
        }
        QVariantList list;
        auto beanNames = getBeanNameForMetaType(metaType);
        for (const auto &beanName : beanNames) {
            McBeanReferencePtr beanRef = McBeanReferencePtr::create();
            beanRef->setName(beanName);
            list << QVariant::fromValue(beanRef);
        }
        builder->addProperty(proName, list);
        return;
    }
    auto mapMetaType = McMapMetaType::fromQMetaType(qmetaType);
    if (mapMetaType.isValid()) {
        if (mapMetaType.keyMetaType() != QMetaType::fromType<QString>()) {
            qCCritical(
                mcIoc(), "Key must be QString. property: %s. class: %s", proName.constData(), metaObj->className());
        } else {
            QMap<QVariant, QVariant> map;
            auto metaType = getMcMetaType(mapMetaType.valueMetaType());
            if (!metaType.isValid()) {
                return;
            }
            auto beanNames = getBeanNameForMetaType(metaType);
            for (const auto &beanName : beanNames) {
                McBeanReferencePtr beanRef = McBeanReferencePtr::create();
                beanRef->setName(beanName);
                map.insert(beanName, QVariant::fromValue(beanRef));
            }
            builder->addProperty(proName, QVariant::fromValue(map));
        }
        return;
    }
    auto metaType = getMcMetaType(qmetaType);
    if (!metaType.isValid()) {
        return;
    }
    auto beanNames = getBeanNameForMetaType(metaType);
    if (beanNames.size() != 1) {
        qCCritical(mcIoc(),
            "If you used MC_RESOURCE(). you must be ensure the type name is uniqued. "
            "error property: %s. class: %s",
            proName.constData(), metaObj->className());
        return;
    }
    auto beanName = *beanNames.constBegin();
    McBeanReferencePtr beanRef = McBeanReferencePtr::create();
    beanRef->setName(beanName);
    builder->addProperty(proName, QVariant::fromValue(beanRef));
}

McMetaType McAnnotationBeanDefinitionReader::getMcMetaType(const QMetaType &qmetaType) const noexcept
{
    McMetaType metaType;
    if (qmetaType.flags().testFlag(QMetaType::IsPointer)) {
        metaType = McMetaType::fromPQMetaType(qmetaType);
    } else {
        metaType = McMetaType::fromSQMetaType(qmetaType);
    }
    return metaType;
}
