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
#include "McIoc/BeanFactory/impl/McAbstractNormalBeanFactory.h"

#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>
#include <QPluginLoader>
#include <QThread>

#include "McIoc/BeanDefinition/IMcBeanDefinition.h"
#include "McIoc/PropertyParser/IMcPropertyParser.h"
#include "McIoc/BeanFactory/impl/McBeanConnector.h"
#include "McIoc/PropertyParser/IMcPropertyConverter.h"

MC_DECL_PRIVATE_DATA(McAbstractNormalBeanFactory)
IMcPropertyConverterPtr converter;
MC_DECL_PRIVATE_DATA_END

McAbstractNormalBeanFactory::McAbstractNormalBeanFactory(QObject *parent)
    : McAbstractBeanFactory(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractNormalBeanFactory);
}

McAbstractNormalBeanFactory::~McAbstractNormalBeanFactory() {}

IMcPropertyConverterPtr McAbstractNormalBeanFactory::getPropertyConverter() const noexcept
{
    return d->converter;
}

void McAbstractNormalBeanFactory::setPropertyConverter(
    IMcPropertyConverterConstPtrRef converter) noexcept
{
    d->converter = converter;
}

QVariant McAbstractNormalBeanFactory::doCreate(IMcBeanDefinitionConstPtrRef beanDefinition,
                                               QThread *thread) noexcept
{
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
        auto isGadget = (QMetaType::type(beanDefinition->getClassName().toLocal8Bit())
                         != QMetaType::UnknownType);
        if (isGadget) {
            return parseOnGadget(beanDefinition);
        }
        auto beanMetaObj = beanDefinition->getBeanMetaObject();
        if (!beanMetaObj) {
            qCritical() << QString("the class '%1' is not in meta-object system")
                               .arg(beanDefinition->getClassName());
            return QVariant();
        }
        QByteArray className = beanMetaObj->className();
        className.append("*");
        auto id = QMetaType::type(className);
        auto builder = McPrivate::IQObjectBuilder::getQObjectBuilder(id);
        if (builder.isNull()) {
            obj = beanMetaObj->newInstance();
        } else {
            obj = builder->create();
        }
    }
    if (!obj) {
        qCritical() << QString("bean '%1' cannot instantiation, please make sure that have a "
                               "non-parameter constructor and declared by Q_INVOKABLE")
                           .arg(beanDefinition->getClassName());
        return QVariant();
    }
    callStartFunction(obj); //!< 调用构造开始函数
    QVariantMap proValues;
    if (!addPropertyValue(obj, beanDefinition, proValues)) {
        qCritical() << QString("failed to init definition '%1'").arg(obj->metaObject()->className());
        return QVariant();
    }
    if (!addObjectConnect(obj, beanDefinition, proValues)) {
        qCritical() << QString("failed to add object connect '%1'")
                           .arg(obj->metaObject()->className());
        return QVariant();
    }
    callFinishedFunction(obj); //!< 调用构造完成函数
    if (thread != nullptr && thread != obj->thread()) {
        obj->moveToThread(thread);
        callThreadFinishedFunction(obj); //!< 调用线程移动结束函数
    }
    auto var = convertToQVariant(obj);
    if (!var.isValid()) {
        return var;
    }
    Qt::ConnectionType conType = Qt::QueuedConnection;
    if (obj->thread() == QThread::currentThread()) {
        conType = Qt::DirectConnection;
    } else if (obj->thread() != QThread::currentThread() && obj->thread()->isRunning()) {
        conType = Qt::BlockingQueuedConnection;
    } else {
        qCritical() << "if you want to moved to other thread. please make sure call QThread::start "
                       "before call getBean/refresh.";
    }
    callTagFunction(obj, MC_STRINGIFY(MC_ALL_FINISHED), conType);
    callTagFunction(obj, MC_STRINGIFY(MC_COMPLETE), conType);
    return var;
}

void McAbstractNormalBeanFactory::callTagFunction(QObject *bean,
                                                  const char *tag,
                                                  Qt::ConnectionType type) noexcept
{
    auto mo = bean->metaObject();
    for(int i = 0; i < mo->methodCount(); ++i) {
        auto method = mo->method(i);
        if (Mc::isContainedTag(method.tag(), tag)) {
            //! 遍历当前对象的所有方法，调用所有被标记过的方法，从超基类开始到子类
            method.invoke(bean, type);
        }
    }
}

void McAbstractNormalBeanFactory::callTagFunction(void *bean,
                                                  const QMetaObject *metaObj,
                                                  const char *tag) noexcept
{
    for (int i = 0; i < metaObj->methodCount(); ++i) {
        auto method = metaObj->method(i);
        if (Mc::isContainedTag(method.tag(), tag)) {
            method.invokeOnGadget(bean);
        }
    }
}

void McAbstractNormalBeanFactory::callStartFunction(QObject *bean) noexcept
{
    callTagFunction(bean, MC_STRINGIFY(MC_BEAN_START));
    callTagFunction(bean, MC_STRINGIFY(MC_STARTED));
}

void McAbstractNormalBeanFactory::callStartFunction(void *bean, const QMetaObject *metaObj) noexcept
{
    callTagFunction(bean, metaObj, MC_STRINGIFY(MC_BEAN_START));
    callTagFunction(bean, metaObj, MC_STRINGIFY(MC_STARTED));
}

QVariant McAbstractNormalBeanFactory::parseOnGadget(
    IMcBeanDefinitionConstPtrRef beanDefinition) noexcept
{
    auto type = QMetaType::type(beanDefinition->getClassName().toLocal8Bit());
    auto bean = QMetaType::create(type);
    if (bean == nullptr) {
        qCritical() << "Cannot make gadget for:" << beanDefinition->getClassName();
        return QVariant();
    }
    auto beanMetaObj = beanDefinition->getBeanMetaObject();
    callStartFunction(bean, beanMetaObj); //!< 调用构造开始函数
    if (!addPropertyValue(bean, beanMetaObj, beanDefinition)) {
        qCritical() << QString("failed to init definition '%1'").arg(beanMetaObj->className());
        return QVariant();
    }
    callFinishedFunction(bean, beanMetaObj); //!< 调用构造完成函数
    return convertToQVariant(bean, beanMetaObj);
}

bool McAbstractNormalBeanFactory::addPropertyValue(QObject *bean,
                                                   IMcBeanDefinitionConstPtrRef beanDefinition,
                                                   QVariantMap &proValues) noexcept
{
    //! 循环给定 bean 的属性集合
    auto props = beanDefinition->getProperties();
    for (auto itr = props.cbegin(); itr != props.cend(); ++itr) {
        //! 获取定义的属性中的对象
        auto value = itr.value();
        
        //! 解析value
        value = d->converter->convert(value);
        proValues.insert(itr.key(), value);
        
        //! 根据给定属性名称获取 给定的bean中的属性对象
        auto index = bean->metaObject()->indexOfProperty(itr.key().toLocal8Bit());
        if(index == -1) {
            qDebug() << QString("bean '%1' cannot found property named for '%2'. it will be a "
                                "dynamic property")
                            .arg(bean->metaObject()->className(), itr.key());
            bean->setProperty(itr.key().toLocal8Bit(), value);
            
        }else{
            auto metaProperty = bean->metaObject()->property(index);
#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
            value.convert(QMetaType::type(metaProperty.typeName()));
#endif
            if (!metaProperty.write(bean, value)) {
                qCritical("bean '%s' write property named for '%s' failure\n"
                          , bean->metaObject()->className()
                          , itr.key().toLocal8Bit().data());
            }
        }
    }
    return true;
}

bool McAbstractNormalBeanFactory::addPropertyValue(
    void *bean, const QMetaObject *metaObj, IMcBeanDefinitionConstPtrRef beanDefinition) noexcept
{
    //! 循环给定 bean 的属性集合
    auto props = beanDefinition->getProperties();
    for (auto itr = props.cbegin(); itr != props.cend(); ++itr) {
        //! 获取定义的属性中的对象
        auto value = itr.value();

        //! 解析value
        value = d->converter->convert(value);
        //! 根据给定属性名称获取 给定的bean中的属性对象
        auto index = metaObj->indexOfProperty(itr.key().toLocal8Bit());
        if (index == -1) {
            qDebug() << QString("bean '%1' cannot found property named for '%2'.")
                            .arg(metaObj->className(), itr.key());
        } else {
            auto metaProperty = metaObj->property(index);
#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
            value.convert(QMetaType::type(metaProperty.typeName()));
#endif
            if (!metaProperty.writeOnGadget(bean, value)) {
                qCritical("bean '%s' write property named for '%s' failure\n",
                          metaObj->className(),
                          itr.key().toLocal8Bit().data());
            }
        }
    }
    return true;
}

bool McAbstractNormalBeanFactory::addObjectConnect(QObject *bean,
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
        QObject *sender;
        QMetaMethod signal;
        QObject *receiver;
        QMetaMethod slot;
        Qt::ConnectionType type;
        
        QString senderProName = con->getSender();
        sender = getPropertyObject(bean, senderProName, proValues);
        if(sender == nullptr) {
            return false;
        }
        auto signalMetaObj = sender->metaObject();
        
        QString signalStr = con->getSignal();
        if (signalStr.startsWith(QString::number(QSIGNAL_CODE))) {
            signalStr.remove(0, 1);
        }
        if(signalStr.isEmpty()) {
            qCritical() << "signal is not exists";
            return false;
        }
        int signalIndex = signalMetaObj->indexOfSignal(signalStr.toLocal8Bit());
        if(signalIndex == -1) {
            qCritical("not exists signal named '%s' for bean '%s'\n",
                      qPrintable(signalStr),
                      signalMetaObj->className());
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
        if (slotStr.startsWith(QString::number(QSLOT_CODE))) {
            slotStr.remove(0, 1);
        }
        if(slotStr.isEmpty()) {
            qCritical() << "slot is not exists";
            return false;
        }
        int slotIndex = slotMetaObj->indexOfMethod(slotStr.toLocal8Bit());
        if(slotIndex == -1) {
            qCritical("not exists slot named '%s' for bean '%s'\n",
                      qPrintable(slotStr),
                      slotMetaObj->className());
            return false;
        }
        slot = slotMetaObj->method(slotIndex);

        type = con->getType();

        QObject::connect(sender, signal, receiver, slot, type);
    }
    return true;
}

QObject *McAbstractNormalBeanFactory::getPropertyObject(QObject *bean,
                                                        const QString &proName,
                                                        const QVariantMap &proValues) noexcept
{
    QObject *obj;
    if (proName == Mc::Constant::Tag::Xml::self) {
        obj = bean;
    } else {
        if(!proValues.contains(proName)) {
            qCritical("not found property named '%s' for bean '%s'\n"
                      , bean->metaObject()->className(), qPrintable(proName));
            return nullptr;
        }
        auto varPro = proValues[proName];
        obj = varPro.value<QObject *>();
    }
    return obj;
}

void McAbstractNormalBeanFactory::callFinishedFunction(QObject *bean) noexcept
{
    callTagFunction(bean, MC_STRINGIFY(MC_BEAN_FINISHED));
    callTagFunction(bean, MC_STRINGIFY(MC_FINISHED));
}

void McAbstractNormalBeanFactory::callFinishedFunction(void *bean,
                                                       const QMetaObject *metaObj) noexcept
{
    callTagFunction(bean, metaObj, MC_STRINGIFY(MC_BEAN_FINISHED));
    callTagFunction(bean, metaObj, MC_STRINGIFY(MC_FINISHED));
}

void McAbstractNormalBeanFactory::callThreadFinishedFunction(QObject *bean) noexcept
{
    callTagFunction(bean, MC_STRINGIFY(MC_THREAD_FINISHED));
    callTagFunction(bean, MC_STRINGIFY(MC_THREAD_MOVED));
}
