#include "McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h"

#include <qmetaobject.h>
#include <QDebug>

#include "McIoc/BeanDefinitionReader/impl/McAnnotationBeanDefinitionReader.h"
#include "McIoc/BeanDefinition/impl/McRootBeanDefinition.h"
#include "McIoc/BeanFactory/impl/McBeanConnector.h"
#include "McIoc/McMacroGlobal.h"

MC_DECL_PRIVATE_DATA(McAnnotationApplicationContext)
/// 用来保存需要自动注入的bean的beanName和BeanDefinition
static QHash<QString, IMcBeanDefinitionPtr> autowiredRegistry;
MC_DECL_PRIVATE_DATA_END

QHash<QString, IMcBeanDefinitionPtr> 
    MC_PRIVATE_DATA_NAME(McAnnotationApplicationContext)::autowiredRegistry 
        = QHash<QString, IMcBeanDefinitionPtr>();

#define globalDefinitions() (MC_PRIVATE_DATA_NAME(McAnnotationApplicationContext)::autowiredRegistry)

McAnnotationApplicationContext::McAnnotationApplicationContext(
        IMcConfigurableBeanFactoryConstPtrRef factory
        , IMcBeanDefinitionReaderConstPtrRef reader
        , QObject *parent)
    : McReadableApplicationContext(factory, reader, parent)
{
    MC_NEW_PRIVATE_DATA(McAnnotationApplicationContext)
}

McAnnotationApplicationContext::McAnnotationApplicationContext(QObject *parent)
    : McReadableApplicationContext(
          IMcBeanDefinitionReaderPtr(new McAnnotationBeanDefinitionReader(globalDefinitions()))
          , parent)
{
    MC_NEW_PRIVATE_DATA(McAnnotationApplicationContext)
}

McAnnotationApplicationContext::~McAnnotationApplicationContext() {
}

void McAnnotationApplicationContext::insertRegistry(const QString &typeName) noexcept {
    Q_ASSERT(!typeName.isEmpty());
    auto isSingleton = true;    //!< 默认为单例
    //! 默认为类型名首字母小写
    auto beanName = typeName.at(0).toLower() + typeName.right(typeName.size() - 1);
    auto type = QMetaType::type(typeName.toLocal8Bit());
    if(type == QMetaType::UnknownType) {
        qCritical() << "it's unregistered for class:" << typeName;
        return;
    }
    //! type存在，metaObj一定不为null
    auto metaObj = QMetaType::metaObjectForType(type);
    auto singletonIndex = metaObj->indexOfClassInfo(MC_SINGLETON);
    auto beanNameIndex = metaObj->indexOfClassInfo(MC_BEANNAME);
    if(singletonIndex != -1) {
        auto classInfo = metaObj->classInfo(singletonIndex);
        bool isTrue = classInfo.value() == QString("true");
        bool isFalse = classInfo.value() == QString("false");
        if(!isTrue && !isFalse) {
            qCritical() << "the singleton value for classInfo must be true or false of string";
        }else{
            isSingleton = isTrue ? true : false;
        }
    }
    if(beanNameIndex != -1) {
        auto classInfo = metaObj->classInfo(beanNameIndex);
        beanName = classInfo.value();
    }
    
    auto beanDefinition = globalDefinitions()[beanName];
    if(!beanDefinition) {
        beanDefinition = McRootBeanDefinitionPtr::create();
        globalDefinitions()[beanName] = beanDefinition;
    }
    if(!beanDefinition->getClassName().isEmpty()) {
        qCritical() << "injected fail. the beanName is exists. it's typeName is" << beanDefinition->getClassName()
                    << ". want inject typeName:" << typeName << "beanName:" << beanName;
        return;
    }
    beanDefinition->setBeanMetaObject(metaObj);
    beanDefinition->setClassName(typeName);
    beanDefinition->setSingleton(isSingleton);
}

void McAnnotationApplicationContext::addConnect(
        const QString &beanName
        , const QString &sender
        , const QString &signal
        , const QString &receiver
        , const QString &slot
        , Qt::ConnectionType type) noexcept {
    
    auto beanDefinition = globalDefinitions()[beanName];
    if(!beanDefinition) {
        beanDefinition = McRootBeanDefinitionPtr::create();
        globalDefinitions()[beanName] = beanDefinition;
    }
    McBeanConnectorPtr connector = McBeanConnectorPtr::create();
    connector->setSender(sender);
    connector->setSignal(signal);
    connector->setReceiver(receiver);
    connector->setSlot(slot);
    connector->setType(type);
    QVariant var;
    var.setValue(connector);
    beanDefinition->addConnector(var);
}
