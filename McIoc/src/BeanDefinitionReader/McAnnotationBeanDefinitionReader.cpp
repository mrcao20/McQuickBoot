#include "McIoc/BeanDefinitionReader/impl/McAnnotationBeanDefinitionReader.h"

#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>

#include "McIoc/BeanFactory/IMcBeanDefinitionRegistry.h"
#include "McIoc/BeanDefinition/IMcBeanDefinition.h"
#include "McIoc/BeanFactory/impl/McBeanReference.h"

MC_DECL_PRIVATE_DATA(McAnnotationBeanDefinitionReader)
QHash<QString, IMcBeanDefinitionPtr> definitions;
MC_DECL_PRIVATE_DATA_END

McAnnotationBeanDefinitionReader::McAnnotationBeanDefinitionReader(
    const QHash<QString, IMcBeanDefinitionPtr> &definitions, QObject *parent)
    : McAbstractBeanDefinitionReader(parent)
{
    MC_NEW_PRIVATE_DATA(McAnnotationBeanDefinitionReader)
            
    d->definitions = definitions;
}

McAnnotationBeanDefinitionReader::~McAnnotationBeanDefinitionReader() 
{
}

void McAnnotationBeanDefinitionReader::doReadBeanDefinition() noexcept 
{
    QHashIterator<decltype (d->definitions)::key_type
            , decltype (d->definitions)::mapped_type> iterator(d->definitions);
    while (iterator.hasNext()) {
        auto item = iterator.next();
        auto key = item.key();
        auto value = item.value();
        inject(key, value);
    }
}

void McAnnotationBeanDefinitionReader::inject(const QString &beanName,
                                              IMcBeanDefinitionConstPtrRef beanDefinition) noexcept
{
    if (registry()->isContained(beanName)) {
        return; //!< 如果存在则不处理
    }
    const QMetaObject *metaObj = beanDefinition->getBeanMetaObject();
    Q_ASSERT_X(metaObj != nullptr,
               "McAnnotationBeanDefinitionReader",
               "if you want to use declarative inject. make sure mark used MC_COMPONENT()");
    injectProperty(metaObj, beanDefinition);
    registry()->registerBeanDefinition(beanName, beanDefinition);
}

void McAnnotationBeanDefinitionReader::injectProperty(
    const QMetaObject *metaObj, IMcBeanDefinitionConstPtrRef beanDefinition) noexcept
{
    for(int i = 0, count = metaObj->classInfoCount(); i < count; ++i) {
        auto classInfo = metaObj->classInfo(i);
        if (qstrcmp(classInfo.name(), MC_AUTOWIRED_TAG) == 0) {
            QString value = classInfo.value();
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
            QStringList list = value.split("=", Qt::SkipEmptyParts);
#else
            QStringList list = value.split("=", QString::SkipEmptyParts);
#endif
            if (list.isEmpty()) {
                continue;
            }
            QString proName, beanName;
            if (list.size() == 1) {
                proName = beanName = list.first().simplified();
            } else {
                proName = list.first().simplified();
                beanName = list.last().simplified();
            }
            if (metaObj->indexOfProperty(proName.toLocal8Bit()) == -1) {
                continue;
            }
            McBeanReferencePtr beanRef = McBeanReferencePtr::create();
            beanRef->setName(beanName);
            beanDefinition->addProperty(proName, QVariant::fromValue(beanRef));
        } else if (qstrcmp(classInfo.name(), MC_RESOURCE_TAG) == 0) {
            QByteArray proName = classInfo.value();
            auto proIndex = metaObj->indexOfProperty(proName);
            if (proIndex == -1) {
                qCritical("Cannot found property: %s. for class: %s",
                          classInfo.value(),
                          metaObj->className());
                continue;
            }
            auto pro = metaObj->property(proIndex);
            auto type = pro.userType();
            auto sharedId = McMetaTypeId::getSequentialValueId(type);
            if (sharedId != -1) {
                QVariantList list;
                auto beanNames = McMetaTypeId::getBeanNamesForId(sharedId);
                for (const auto &beanName : beanNames) {
                    McBeanReferencePtr beanRef = McBeanReferencePtr::create();
                    beanRef->setName(beanName);
                    list << QVariant::fromValue(beanRef);
                }
                beanDefinition->addProperty(proName, list);
                continue;
            }
            auto mapIds = McMetaTypeId::associativeIds();
            if (mapIds.contains(type)) {
                auto mapId = mapIds.value(type);
                if (mapId->keyId != QMetaType::QString) {
                    qCritical("Key must be QString. property: %s. class: %s",
                              classInfo.value(),
                              metaObj->className());
                } else {
                    QMap<QVariant, QVariant> map;
                    sharedId = mapId->valueId;
                    auto beanNames = McMetaTypeId::getBeanNamesForId(sharedId);
                    for (const auto &beanName : beanNames) {
                        McBeanReferencePtr beanRef = McBeanReferencePtr::create();
                        beanRef->setName(beanName);
                        map.insert(beanName, QVariant::fromValue(beanRef));
                    }
                    beanDefinition->addProperty(proName, QVariant::fromValue(map));
                }
                continue;
            }
            sharedId = type;
            QByteArray typeName = pro.typeName();
            if (typeName.endsWith("*")) {
                typeName.remove(typeName.length() - 1, 1);
                typeName.append("Ptr");
                sharedId = QMetaType::type(typeName);
            }
            auto beanNames = McMetaTypeId::getBeanNamesForId(sharedId);
            if (beanNames.length() != 1) {
                qCritical("If you used MC_RESOURCE(). you must be ensure the type name is uniqued. "
                          "error property: %s. class: %s",
                          classInfo.value(),
                          metaObj->className());
                continue;
            }
            auto beanName = beanNames.first();
            McBeanReferencePtr beanRef = McBeanReferencePtr::create();
            beanRef->setName(beanName);
            beanDefinition->addProperty(proName, QVariant::fromValue(beanRef));
        } else {
            continue;
        }
    }
}
