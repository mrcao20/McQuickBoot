#include "include/BeanDefinitionReader/impl/McAnnotationBeanDefinitionReader.h"

#include <QMetaObject>
#include <QMetaProperty>

#include "include/BeanFactory/IMcBeanDefinitionRegistry.h"
#include "include/BeanDefinition/IMcBeanDefinition.h"
#include "include/BeanFactory/impl/McBeanReference.h"

MC_DECL_PRIVATE_DATA(McAnnotationBeanDefinitionReader)
QHash<QString, IMcBeanDefinitionPtr> definitions;
MC_DECL_PRIVATE_DATA_END

McAnnotationBeanDefinitionReader::McAnnotationBeanDefinitionReader(
        const QHash<QString, IMcBeanDefinitionPtr> &definitions
        , QObject *parent)
    : McAbstractBeanDefinitionReader(parent)
{
    MC_NEW_PRIVATE_DATA(McAnnotationBeanDefinitionReader)
            
    d->definitions = definitions;
}

McAnnotationBeanDefinitionReader::~McAnnotationBeanDefinitionReader() {
}

void McAnnotationBeanDefinitionReader::doReadBeanDefinition() noexcept {
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
        const QString& beanName
        , IMcBeanDefinitionConstPtrRef beanDefinition) noexcept {
    
	const QMetaObject *metaObj = beanDefinition->getBeanMetaObject();
	Q_ASSERT(metaObj != nullptr);
	injectProperty(metaObj, beanDefinition);
	registry()->registerBeanDefinition(beanName, beanDefinition);
}

void McAnnotationBeanDefinitionReader::injectProperty(
        const QMetaObject *metaObj
        , IMcBeanDefinitionConstPtrRef beanDefinition) noexcept {
    
	int count = metaObj->propertyCount();
	for (int i = 0; i < count; ++i) {
		QMetaProperty prop = metaObj->property(i);
        auto isUser = prop.isUser();
        auto beanName = prop.name();
		int classInfoIndex = metaObj->indexOfClassInfo(beanName);
        if(classInfoIndex != -1) {
            isUser = true;
            auto classInfo = metaObj->classInfo(classInfoIndex);
            beanName = classInfo.value();
        }
        if(!isUser) {
            continue;
        }
        
        McBeanReferencePtr beanRef = McBeanReferencePtr::create();
        beanRef->setName(beanName);
		beanDefinition->addProperty(prop.name(), QVariant::fromValue(beanRef));
	}
}
