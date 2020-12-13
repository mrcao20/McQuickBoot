#include "McIoc/BeanDefinitionReader/impl/McAnnotationBeanDefinitionReader.h"

#include <QMetaObject>
#include <QMetaProperty>

#include "McIoc/BeanFactory/IMcBeanDefinitionRegistry.h"
#include "McIoc/BeanDefinition/IMcBeanDefinition.h"
#include "McIoc/BeanFactory/impl/McBeanReference.h"

MC_DECL_PRIVATE_DATA(McAnnotationBeanDefinitionReader)
QHash<QString, IMcBeanDefinitionPtr> definitions;
MC_DECL_PRIVATE_DATA_END

McAnnotationBeanDefinitionReader::McAnnotationBeanDefinitionReader(
        const QHash<QString, IMcBeanDefinitionPtr> &definitions,
        QObject *parent)
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

void McAnnotationBeanDefinitionReader::inject(
        const QString& beanName,
        IMcBeanDefinitionConstPtrRef beanDefinition) noexcept 
{
	const QMetaObject *metaObj = beanDefinition->getBeanMetaObject();
	Q_ASSERT_X(metaObj != nullptr
            , "McAnnotationBeanDefinitionReader"
               , "if you want to use declarative inject. make sure mark used MC_COMPONENT");
	injectProperty(metaObj, beanDefinition);
	registry()->registerBeanDefinition(beanName, beanDefinition);
}

void McAnnotationBeanDefinitionReader::injectProperty(
        const QMetaObject *metaObj,
        IMcBeanDefinitionConstPtrRef beanDefinition) noexcept 
{
    for(int i = 0, count = metaObj->classInfoCount(); i < count; ++i) {
        auto classInfo = metaObj->classInfo(i);
        if(qstrcmp(classInfo.name(), MC_AUTOWIRED_TAG) != 0) {
            continue;
        }
        QString value = classInfo.value();
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        QStringList list = value.split("=", Qt::SkipEmptyParts);
#else
        QStringList list = value.split("=", QString::SkipEmptyParts);
#endif
        if(list.isEmpty()) {
            continue;
        }
        QString proName, beanName;
        if(list.size() == 1) {
            proName = beanName = list.first().simplified();
        } else {
            proName = list.first().simplified();
            beanName = list.last().simplified();
        }
        if(metaObj->indexOfProperty(proName.toLocal8Bit()) != -1) {
            McBeanReferencePtr beanRef = McBeanReferencePtr::create();
            beanRef->setName(beanName);
            beanDefinition->addProperty(proName, QVariant::fromValue(beanRef));
        }
    }
}
