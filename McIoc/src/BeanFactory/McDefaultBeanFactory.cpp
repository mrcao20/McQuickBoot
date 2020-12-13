#include "McIoc/BeanFactory/impl/McDefaultBeanFactory.h"

#include <QPluginLoader>
#include <QMetaObject>
#include <QMetaProperty>
#include <QDebug>

#include "McIoc/BeanDefinition/IMcBeanDefinition.h"
#include "McIoc/PropertyParser/IMcPropertyParser.h"
#include "McIoc/BeanFactory/impl/McBeanConnector.h"
#include "McIoc/PropertyParser/IMcPropertyConverter.h"
#include "McIoc/Thread/IMcDeleteThreadWhenQuit.h"
#include "McIoc/Destroyer/IMcDestroyer.h"

MC_DECL_PRIVATE_DATA(McDefaultBeanFactory)
IMcPropertyConverterPtr converter;
MC_DECL_PRIVATE_DATA_END

McDefaultBeanFactory::McDefaultBeanFactory(
        IMcPropertyConverterConstPtrRef converter,
        QObject *parent)
    : McAbstractBeanFactory(parent)
{
    MC_NEW_PRIVATE_DATA(McDefaultBeanFactory);
    
    d->converter = converter;
}

McDefaultBeanFactory::~McDefaultBeanFactory() 
{
}

QVariant McDefaultBeanFactory::doCreate(
        IMcBeanDefinitionConstPtrRef beanDefinition,
        QThread *thread) noexcept 
{
    QVariant var;
    QObject *obj = nullptr;
    auto pluginPath = beanDefinition->getPluginPath();
    if(!pluginPath.isEmpty()){
        QPluginLoader loader(pluginPath);
        if (!loader.load()) {
            qWarning() << pluginPath << "cannot load!!";
            return QVariant();
        }
        obj = loader.instance();
    }else{
        auto beanMetaObj = beanDefinition->getBeanMetaObject();
        if (!beanMetaObj) {
            qCritical() << QString("the class '%1' is not in meta-object system").arg(beanDefinition->getClassName());
            return QVariant();
        }
        obj = beanMetaObj->newInstance();
    }
    if (!obj) {
		qCritical() << QString("bean '%1' cannot instantiation, please make sure that have a non-parameter constructor and declared by Q_INVOKABLE")
			.arg(beanDefinition->getClassName());
		return QVariant();
	}
    QObjectPtr bean(obj, Mc::McCustomDeleter());
    callStartFunction(bean);    //!< 调用构造开始函数
    QVariantMap proValues;
	if (!addPropertyValue(bean, beanDefinition, proValues)) {
		qCritical() << QString("failed to init definition '%1'").arg(bean->metaObject()->className());
        return QVariant();
	}
    if(!addObjectConnect(bean, beanDefinition, proValues)) {
        qCritical() << QString("failed to add object connect '%1'").arg(bean->metaObject()->className());
        return QVariant();
    }
    callFinishedFunction(bean);     //!< 调用构造完成函数
    if(thread != nullptr && thread != bean->thread()) {
        bean->moveToThread(thread);
        callThreadFinishedFunction(bean);   //!< 调用线程移动结束函数
    }
    var.setValue(bean);
    QString typeName = QString("%1Ptr").arg(bean->metaObject()->className());
    if(!var.convert(QMetaType::type(typeName.toLocal8Bit()))) {
        qCritical() << QString("failed convert QObjectPtr to '%1'").arg(typeName);
        return QVariant();
    }
    auto destoryer = var.value<IMcDeleteThreadWhenQuitPtr>();
    if(destoryer) {
        destoryer->deleteWhenQuit();
    }
    auto customDeleter = var.value<IMcDestroyerPtr>();
    if(!customDeleter.isNull()) {
        //! 这里如果传递共享指针，那么该对象将永远不会析构
        bean->setProperty(MC_CUSTOM_DELETER_PROPERTY_NAME, QVariant::fromValue(customDeleter.data()));
    }
    return var;
}

void McDefaultBeanFactory::callTagFunction(QObjectConstPtrRef bean, const char *tag) noexcept 
{
    auto mo = bean->metaObject();
    for(int i = 0; i < mo->methodCount(); ++i) {
        auto method = mo->method(i);
        QString tags = method.tag();
        if(tags.contains(tag)) {
            method.invoke(bean.data(), Qt::DirectConnection);
//            break;    //!< 遍历当前对象的所有方法，调用所有被标记过的方法，从超基类开始到子类
        }
    }
}

void McDefaultBeanFactory::callStartFunction(QObjectConstPtrRef bean) noexcept 
{
    callTagFunction(bean, MC_MACRO_STR(MC_BEAN_START));
}

bool McDefaultBeanFactory::addPropertyValue(QObjectConstPtrRef bean,
                                            IMcBeanDefinitionConstPtrRef beanDefinition,
                                            QVariantMap &proValues) noexcept 
{
    //! 循环给定 bean 的属性集合
	auto props = beanDefinition->getProperties();
	for (auto itr = props.cbegin(); itr != props.cend(); ++itr) {
        //! 获取定义的属性中的对象
		auto value = itr.value();
        
		//! 解析value
        value = d->converter->convert(this, value);
        proValues.insert(itr.key(), value);
        
		//! 根据给定属性名称获取 给定的bean中的属性对象
        auto index = bean->metaObject()->indexOfProperty(itr.key().toLocal8Bit());
        if(index == -1) {
            qDebug() << QString("bean '%1' cannot found property named for '%2'. it will be a dynamic property")
                           .arg(bean->metaObject()->className(), itr.key());
            bean->setProperty(itr.key().toLocal8Bit(), value);
            
        }else{
            auto metaProperty = bean->metaObject()->property(index);
#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
            value.convert(QMetaType::type(metaProperty.typeName()));
#endif
            if(!metaProperty.write(bean.data(), value)) {
                qCritical("bean '%s' write property named for '%s' failure\n"
                          , bean->metaObject()->className()
                          , itr.key().toLocal8Bit().data());
            }
        }
	}
	return true;
}

bool McDefaultBeanFactory::addObjectConnect(QObjectConstPtrRef bean,
                                            IMcBeanDefinitionConstPtrRef beanDefinition,
                                            const QVariantMap &proValues) noexcept 
{
    auto connectors = beanDefinition->getConnectors();
    for(auto connector : connectors) {
        McBeanConnectorPtr con = connector.value<McBeanConnectorPtr>();
        if(!con) {
            qCritical("has a connector, but cannot convert to McBeanConnectorPtr for bean '%s'", bean->metaObject()->className());
            return false;
        }
        QObjectPtr sender;
        QMetaMethod signal;
        QObjectPtr receiver;
        QMetaMethod slot;
        Qt::ConnectionType type;
        
        QString senderProName = con->getSender();
        sender = getPropertyObject(bean, senderProName, proValues);
        if(sender == nullptr) {
            return false;
        }
        auto signalMetaObj = sender->metaObject();
        
        QString signalStr = con->getSignal();
        if(signalStr.isEmpty()) {
            qCritical() << "signal is not exists";
            return false;
        }
        int signalIndex = signalMetaObj->indexOfSignal(signalStr.toLocal8Bit());
        if(signalIndex == -1) {
            qCritical("not exists signal named '%s' for bean '%s'\n", signalMetaObj->className(), qPrintable(signalStr));
            return false;
        }
        signal = signalMetaObj->method(signalIndex);
        
        QString receiverProName = con->getReceiver();
        receiver = getPropertyObject(bean, receiverProName, proValues);
        if(receiver == nullptr) {
            return false;
        }
        auto slotMetaObj = receiver->metaObject();
        
        QString slotStr = con->getSlot();
        if(slotStr.isEmpty()) {
            qCritical() << "slot is not exists";
            return false;
        }
        int slotIndex = slotMetaObj->indexOfMethod(slotStr.toLocal8Bit());
        if(slotIndex == -1) {
            qCritical("not exists slot named '%s' for bean '%s'\n", slotMetaObj->className(), qPrintable(slotStr));
            return false;
        }
        slot = slotMetaObj->method(slotIndex);
        
        type = con->getType();
        
        QObject::connect(sender.data(), signal, receiver.data(), slot, type);
    }
    return true;
}

QObjectPtr McDefaultBeanFactory::getPropertyObject(QObjectConstPtrRef bean,
                                                   const QString &proName,
                                                   const QVariantMap &proValues) noexcept 
{
    QObjectPtr obj;
    if(proName == MC_THIS_TAG) {
        obj = bean;
    }else{
        if(!proValues.contains(proName)) {
            qCritical("not found property named '%s' for bean '%s'\n"
                      , bean->metaObject()->className(), qPrintable(proName));
            return QObjectPtr();
        }
        auto varPro = proValues[proName];
        obj = varPro.value<QObjectPtr>();
    }
    return obj;
}

void McDefaultBeanFactory::callFinishedFunction(QObjectConstPtrRef bean) noexcept 
{
    callTagFunction(bean, MC_MACRO_STR(MC_BEAN_FINISHED));
}

void McDefaultBeanFactory::callThreadFinishedFunction(QObjectConstPtrRef bean) noexcept 
{
    callTagFunction(bean, MC_MACRO_STR(MC_THREAD_FINISHED));
}
