#include "McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h"

#include <QMetaClassInfo>
#include <QGlobalStatic>
#include <QDebug>

#include "McIoc/BeanDefinitionReader/impl/McAnnotationBeanDefinitionReader.h"
#include "McIoc/BeanDefinition/impl/McRootBeanDefinition.h"
#include "McIoc/BeanFactory/impl/McBeanConnector.h"
#include "McIoc/BeanFactory/impl/McMetaTypeId.h"
#include "McIoc/McMacroGlobal.h"

MC_DECL_PRIVATE_DATA(McAnnotationApplicationContext)
MC_DECL_PRIVATE_DATA_END

//! 用来保存需要自动注入的bean的beanName和BeanDefinition
typedef QHash<QString, IMcBeanDefinitionPtr> McBeanDefinitionContainter; 
MC_GLOBAL_STATIC(McBeanDefinitionContainter, mcAutowiredRegistry)

MC_STATIC()
MC_DESTROY()
mcAutowiredRegistry->clear();
MC_STATIC_END

McAnnotationApplicationContext::McAnnotationApplicationContext(
    IMcConfigurableBeanFactoryConstPtrRef factory,
    IMcBeanDefinitionReaderConstPtrRef reader,
    QObject *parent)
    : McReadableApplicationContext(factory, reader, parent)
{
    MC_NEW_PRIVATE_DATA(McAnnotationApplicationContext);
}

McAnnotationApplicationContext::McAnnotationApplicationContext(
    IMcBeanDefinitionReaderConstPtrRef reader, QObject *parent)
    : McReadableApplicationContext(reader, parent)
{
    MC_NEW_PRIVATE_DATA(McAnnotationApplicationContext);
}

McAnnotationApplicationContext::McAnnotationApplicationContext(QObject *parent)
    : McReadableApplicationContext(parent)
{
    MC_NEW_PRIVATE_DATA(McAnnotationApplicationContext);
    
    //! 确保只会被调用一次，并且调用时间在QCoreApplication之后
    //! C++11之后编译器必须保证静态局部变量的初始化的线程安全性
    static int init = []() {
        McAnnotationApplicationContext::init();
        return 0;
    }();
    Q_UNUSED(init)

    generateReader();
}

McAnnotationApplicationContext::~McAnnotationApplicationContext() 
{
}

void McAnnotationApplicationContext::generateReader() noexcept
{
    auto reader = McAnnotationBeanDefinitionReaderPtr::create(*mcAutowiredRegistry);
    setReader(reader);
}

void McAnnotationApplicationContext::init() noexcept
{
    auto qobjectMetaTypeIds = McMetaTypeId::qobjectPointerIds();
    for (auto type : qobjectMetaTypeIds.keys()) {
        initBeanDefinition(type);
    }
    auto gadgetIds = McMetaTypeId::gadgetIds();
    for (auto type : gadgetIds.keys()) {
        initBeanDefinition(type);
    }
}

void McAnnotationApplicationContext::initBeanDefinition(int type) noexcept
{
    Q_ASSERT_X(QMetaType::isRegistered(type),
               "McAnnotationApplicationContext",
               "type not registered");
    auto metaObj = QMetaType::metaObjectForType(type);
    Q_ASSERT_X(metaObj, "McAnnotationApplicationContext", "cannot get meta object");
    auto componentIndex = metaObj->indexOfClassInfo(MC_COMPONENT_TAG);
    if (componentIndex == -1) {
        return;
    }

    QString beanName = McPrivate::getBeanName(metaObj);
    McBeanDefinitionContainter *ar = mcAutowiredRegistry;
    auto beanDefinition = (*ar)[beanName];
    if (!beanDefinition) {
        beanDefinition = McRootBeanDefinitionPtr::create();
        (*ar)[beanName] = beanDefinition;
    }
    if (!beanDefinition->getClassName().isEmpty()) {
        return;
    }
    auto isSingleton = true; //!< 默认为单例
    auto singletonIndex = metaObj->indexOfClassInfo(MC_SINGLETON_TAG);
    if (singletonIndex != -1) {
        auto classInfo = metaObj->classInfo(singletonIndex);
        bool isTrue = classInfo.value() == QString("true");
        bool isFalse = classInfo.value() == QString("false");
        if (!isTrue && !isFalse) {
            qCritical() << "the singleton value for classInfo must be true or false of string";
        } else {
            isSingleton = isTrue ? true : false;
        }
    }
    bool isPointer = false; //! 默认不为原始指针
    auto pointerIndex = metaObj->indexOfClassInfo(MC_POINTER_TAG);
    if (pointerIndex != -1) {
        auto classInfo = metaObj->classInfo(pointerIndex);
        bool isTrue = classInfo.value() == QString("true");
        bool isFalse = classInfo.value() == QString("false");
        if (!isTrue && !isFalse) {
            qCritical() << "the pointer value for classInfo must be true or false of string";
        } else {
            isPointer = isTrue ? true : false;
        }
    }
    beanDefinition->setBeanMetaObject(metaObj);
    beanDefinition->setClassName(metaObj->className());
    beanDefinition->setSingleton(isSingleton);
    beanDefinition->setPointer(isPointer);
}

void McAnnotationApplicationContext::addConnect(const QString &beanName,
                                                const QString &sender,
                                                const QString &signal,
                                                const QString &receiver,
                                                const QString &slot,
                                                Qt::ConnectionType type) noexcept
{
    McBeanDefinitionContainter *ar = mcAutowiredRegistry;
    auto beanDefinition = (*ar)[beanName];
    if(!beanDefinition) {
        beanDefinition = McRootBeanDefinitionPtr::create();
        (*ar)[beanName] = beanDefinition;
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
