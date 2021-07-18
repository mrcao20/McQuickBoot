#include "McWidgetIoc/BeanFactory/Impl/McDefaultWidgetBeanFactory.h"

#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>
#include <QPluginLoader>
#include <QWidget>

#include <McIoc/BeanDefinition/IMcBeanDefinition.h>
#include <McIoc/BeanDefinition/IMcCustomPlaceholder.h>
#include <McIoc/BeanDefinition/impl/McRootBeanDefinition.h>
#include <McIoc/BeanFactory/impl/McBeanConnector.h>
#include <McIoc/BeanFactory/impl/McBeanEnum.h>
#include <McIoc/BeanFactory/impl/McBeanPlaceholder.h>
#include <McIoc/BeanFactory/impl/McBeanReference.h>
#include <McIoc/PropertyParser/IMcPropertyConverter.h>

#include "McWidgetIoc/BuiltInBeanBuilder/IMcBuiltInBeanBuilder.h"
#include "McWidgetIoc/BuiltInBeanBuilder/McBuiltInBeanContainer.h"

MC_DECL_PRIVATE_DATA(McDefaultWidgetBeanFactory)
IMcPropertyConverterPtr converter;
MC_DECL_PRIVATE_DATA_END

McDefaultWidgetBeanFactory::McDefaultWidgetBeanFactory(QObject *parent)
    : McAbstractWidgetBeanFactory(parent)
{
    MC_NEW_PRIVATE_DATA(McDefaultWidgetBeanFactory);
}

McDefaultWidgetBeanFactory::~McDefaultWidgetBeanFactory() {}

void McDefaultWidgetBeanFactory::setConverter(IMcPropertyConverterConstPtrRef converter) noexcept
{
    d->converter = converter;
}

QWidget *McDefaultWidgetBeanFactory::doCreate(IMcBeanDefinitionConstPtrRef beanDefinition) const
    noexcept
{
    QObject *obj = nullptr;
    auto pluginPath = beanDefinition->getPluginPath();
    if (!pluginPath.isEmpty()) {
        QPluginLoader loader(pluginPath);
        if (!loader.load()) {
            qCWarning(mcWidgetIoc()) << pluginPath << "cannot load!!";
            return nullptr;
        }
        obj = loader.instance();
    } else {
        auto beanMetaObj = beanDefinition->getBeanMetaObject();
        if (!beanMetaObj) {
            qCCritical(mcWidgetIoc()) << QString("the class '%1' is not in meta-object system")
                                             .arg(beanDefinition->getClassName());
            return nullptr;
        }
        QByteArray className = beanMetaObj->className();
        auto beanBuilder = McBuiltInBeanContainer::getBuilder(className);
        if (!beanBuilder.isNull()) {
            auto w = beanBuilder->build();
            addBuiltInProperty(w, beanBuilder, beanDefinition);
            return w;
        }
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
        qCCritical(mcWidgetIoc())
            << QString("bean '%1' cannot instantiation, please make sure that have a "
                       "non-parameter constructor and declared by Q_INVOKABLE")
                   .arg(beanDefinition->getClassName());
        return nullptr;
    }
    QWidget *widget = qobject_cast<QWidget *>(obj);
    if (widget == nullptr) {
        qCCritical(mcWidgetIoc()) << "cannot instantiation non qwidget class:"
                                  << beanDefinition->getClassName();
        return nullptr;
    }
    callStartFunction(obj); //!< 调用构造开始函数
    QVariantMap proValues;
    if (!addPropertyValue(widget, beanDefinition, proValues)) {
        qCCritical(mcWidgetIoc()) << QString("failed to init definition '%1'")
                                         .arg(obj->metaObject()->className());
        return nullptr;
    }
    if (!addObjectConnect(obj, beanDefinition, proValues)) {
        qCCritical(mcWidgetIoc()) << QString("failed to add object connect '%1'")
                                         .arg(obj->metaObject()->className());
        return nullptr;
    }
    callFinishedFunction(obj); //!< 调用构造完成函数
    callTagFunction(obj, MC_STRINGIFY(MC_ALL_FINISHED));
    callTagFunction(obj, MC_STRINGIFY(MC_COMPLETE));
    return widget;
}

void McDefaultWidgetBeanFactory::callTagFunction(QObject *bean,
                                                 const char *tag,
                                                 Qt::ConnectionType type) const noexcept
{
    auto mo = bean->metaObject();
    for (int i = 0; i < mo->methodCount(); ++i) {
        auto method = mo->method(i);
        if (Mc::isContainedTag(method.tag(), tag)) {
            //! 遍历当前对象的所有方法，调用所有被标记过的方法，从超基类开始到子类
            method.invoke(bean, type);
        }
    }
}

void McDefaultWidgetBeanFactory::callStartFunction(QObject *bean) const noexcept
{
    callTagFunction(bean, MC_STRINGIFY(MC_BEAN_START));
    callTagFunction(bean, MC_STRINGIFY(MC_STARTED));
}

bool McDefaultWidgetBeanFactory::addPropertyValue(QWidget *bean,
                                                  IMcBeanDefinitionConstPtrRef beanDefinition,
                                                  QVariantMap &proValues) const noexcept
{
    //! 循环给定 bean 的属性集合
    auto props = beanDefinition->getProperties();
    for (auto itr = props.cbegin(); itr != props.cend(); ++itr) {
        //! 获取定义的属性中的对象
        auto value = itr.value();

        //! 解析value
        value = convert(value, bean);
        proValues.insert(itr.key(), value);

        //! 根据给定属性名称获取 给定的bean中的属性对象
        auto index = bean->metaObject()->indexOfProperty(itr.key().toLocal8Bit());
        if (index == -1) {
            qCDebug(mcWidgetIoc())
                << QString("bean '%1' cannot found property named for '%2'. it will be a "
                           "dynamic property")
                       .arg(bean->metaObject()->className(), itr.key());
            bean->setProperty(itr.key().toLocal8Bit(), value);

        } else {
            auto metaProperty = bean->metaObject()->property(index);
#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
            value.convert(QMetaType::type(metaProperty.typeName()));
#endif
            if (!metaProperty.write(bean, value)) {
                qCCritical(mcWidgetIoc(),
                           "bean '%s' write property named for '%s' failure\n",
                           bean->metaObject()->className(),
                           itr.key().toLocal8Bit().data());
            }
        }
    }
    return true;
}

void McDefaultWidgetBeanFactory::addBuiltInProperty(QWidget *bean,
                                                    IMcBuiltInBeanBuilderConstPtrRef builder,
                                                    IMcBeanDefinitionConstPtrRef beanDefinition) const
    noexcept
{
    auto props = beanDefinition->getProperties();
    for (auto itr = props.cbegin(); itr != props.cend(); ++itr) {
        //! 获取定义的属性中的对象
        auto value = itr.value();

        //! 解析value
        value = convert(value, bean);
        builder->setProperty(itr.key(), value);
    }
}

bool McDefaultWidgetBeanFactory::addObjectConnect(QObject *bean,
                                                  IMcBeanDefinitionConstPtrRef beanDefinition,
                                                  const QVariantMap &proValues) const noexcept
{
    auto connectors = beanDefinition->getConnectors();
    for (auto connector : connectors) {
        McBeanConnectorPtr con = connector.value<McBeanConnectorPtr>();
        if (!con) {
            qCCritical(mcWidgetIoc(),
                       "has a connector, but cannot convert to McBeanConnectorPtr for bean '%s'",
                       bean->metaObject()->className());
            return false;
        }
        QObject *sender;
        QMetaMethod signal;
        QObject *receiver;
        QMetaMethod slot;
        Qt::ConnectionType type;

        QString senderProName = con->getSender();
        sender = getPropertyObject(bean, senderProName, proValues);
        if (sender == nullptr) {
            return false;
        }
        auto signalMetaObj = sender->metaObject();

        QString signalStr = con->getSignal();
        if (signalStr.startsWith(QString::number(QSIGNAL_CODE))) {
            signalStr.remove(0, 1);
        }
        if (signalStr.isEmpty()) {
            qCritical() << "signal is not exists";
            return false;
        }
        int signalIndex = signalMetaObj->indexOfSignal(signalStr.toLocal8Bit());
        if (signalIndex == -1) {
            qCritical("not exists signal named '%s' for bean '%s'\n",
                      qPrintable(signalStr),
                      signalMetaObj->className());
            return false;
        }
        signal = signalMetaObj->method(signalIndex);

        QString receiverProName = con->getReceiver();
        receiver = getPropertyObject(bean, receiverProName, proValues);
        if (receiver == nullptr) {
            return false;
        }
        auto slotMetaObj = receiver->metaObject();

        QString slotStr = con->getSlot();
        if (slotStr.startsWith(QString::number(QSLOT_CODE))) {
            slotStr.remove(0, 1);
        }
        if (slotStr.isEmpty()) {
            qCritical() << "slot is not exists";
            return false;
        }
        int slotIndex = slotMetaObj->indexOfMethod(slotStr.toLocal8Bit());
        if (slotIndex == -1) {
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

QObject *McDefaultWidgetBeanFactory::getPropertyObject(QObject *bean,
                                                       const QString &proName,
                                                       const QVariantMap &proValues) const noexcept
{
    QObject *obj;
    if (proName == Mc::Constant::Tag::Xml::self) {
        obj = bean;
    } else {
        if (!proValues.contains(proName)) {
            qCCritical(mcWidgetIoc(),
                       "not found property named '%s' for bean '%s'\n",
                       bean->metaObject()->className(),
                       qPrintable(proName));
            return nullptr;
        }
        auto varPro = proValues[proName];
        obj = varPro.value<QObject *>();
    }
    return obj;
}

void McDefaultWidgetBeanFactory::callFinishedFunction(QObject *bean) const noexcept
{
    callTagFunction(bean, MC_STRINGIFY(MC_BEAN_FINISHED));
    callTagFunction(bean, MC_STRINGIFY(MC_FINISHED));
}

void McDefaultWidgetBeanFactory::callThreadFinishedFunction(QObject *bean) noexcept
{
    callTagFunction(bean, MC_STRINGIFY(MC_THREAD_FINISHED));
    callTagFunction(bean, MC_STRINGIFY(MC_THREAD_MOVED));
}

QVariant McDefaultWidgetBeanFactory::convert(const QVariant &value, QWidget *parent) const noexcept
{
    if (value.canConvert<QVariantList>()) {
        return convertList(value, parent);
    } else if (value.canConvert<QMap<QVariant, QVariant>>()) {
        return convertMap(value, parent);
    } else if (value.canConvert<McBeanReferencePtr>()) {
        return convertRef(value, parent);
    } else if (!d->converter.isNull()) {
        return d->converter->convert(value);
    } else {
        return QVariant();
    }
}

QVariant McDefaultWidgetBeanFactory::convertRef(const QVariant &value, QWidget *parent) const
    noexcept
{
    auto ref = value.value<McBeanReferencePtr>();
    if (ref.isNull()) { //!< 判断是否能够成功转换
        //! 失败，记录错误信息
        qCCritical(mcWidgetIoc()) << "cannot inject beanReference";
        return QVariant();
    }
    auto pluginPath = ref->getPluginPath();
    if (!pluginPath.isEmpty()) {
        McRootBeanDefinitionPtr def = McRootBeanDefinitionPtr::create();
        def->setPluginPath(pluginPath);
        auto widget = doCreate(def);
        if (widget == nullptr) {
            qCWarning(mcWidgetIoc())
                << QString("failed to create bean of plugin '%1'").arg(pluginPath);
            return QVariant();
        }
        widget->setParent(parent);
        return QVariant::fromValue(widget);
    } else {
        auto name = ref->getName();
        if (isContained(name)) {
            auto def = getBeanDefinitions().value(name);
            auto metaObj = def->getBeanMetaObject();
            if (metaObj != nullptr && !metaObj->inherits(&QWidget::staticMetaObject)) {
                return d->converter->convert(value);
            }
        }
        return QVariant::fromValue(getBean(ref->getName(), parent));
    }
}

QVariant McDefaultWidgetBeanFactory::convertList(const QVariant &value, QWidget *parent) const
    noexcept
{
    QVariantList result;

    auto list = value.value<QVariantList>();

    for (const auto &var : list) {
        result << convert(var, parent);
    }

    return result;
}

QVariant McDefaultWidgetBeanFactory::convertMap(const QVariant &value, QWidget *parent) const
    noexcept
{
    QMap<QVariant, QVariant> result;

    auto map = value.value<QMap<QVariant, QVariant>>();

    QMapIterator<decltype(map)::key_type, decltype(map)::mapped_type> itr(map);
    while (itr.hasNext()) {
        auto item = itr.next();
        auto key = item.key();
        auto value = item.value();

        QVariant resultKey, resultValue;

        resultValue = convert(value, parent);
        if (key.userType() == qMetaTypeId<McBeanPlaceholderPtr>()) {
            auto valueObj = resultValue.value<QObject *>();
            auto keyPlh = key.value<McBeanPlaceholderPtr>();
            auto plh = keyPlh->getPlaceholder();
            if (plh == "className") {
                resultKey = valueObj->metaObject()->className();
            } else if (plh == "objectName") {
                resultKey = valueObj->objectName();
            } else if (plh == "custom") {
                auto customPlh = resultValue.value<IMcCustomPlaceholderPtr>();
                resultKey = customPlh->getKey();
            } else {
                qCCritical(mcWidgetIoc()) << "the plh must be one of className/objectName/custom";
            }
        } else {
            resultKey = convert(key, parent);
        }

        if (resultKey.isValid() && resultValue.isValid()) {
            result.insert(resultKey, resultValue);
        }
    }

    return QVariant::fromValue(result);
}
