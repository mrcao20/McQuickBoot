#include "McBoot/Configuration/McConfigurationContainer.h"

#include <QDebug>
#include <QMetaClassInfo>

#include <McIoc/ApplicationContext/IMcApplicationContext.h>
#include <McIoc/BeanFactory/impl/McMetaTypeId.h>

#include "McBoot/Configuration/McSimpleBeanDefinition.h"
#include "McBoot/IMcQuickBoot.h"

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(McConfigurationContainer)
MC_STATIC_END

MC_DECL_PRIVATE_DATA(McConfigurationContainer)
MC_DECL_PRIVATE_DATA_END

McConfigurationContainer::McConfigurationContainer(QObject *parent) noexcept
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McConfigurationContainer);
}

McConfigurationContainer::~McConfigurationContainer()
{
}

void McConfigurationContainer::init(const IMcQuickBoot *boot) noexcept
{
    auto appCtx = boot->getApplicationContext();
    auto beanNames = Mc::getComponents(appCtx, MC_CONFIGURATION_TAG);
    auto sharedIds = McMetaTypeId::sharedPointerIds();
    for (const auto &beanName : beanNames) {
        auto obj = appCtx->getBean(beanName);
        if (!obj) {
            qCritical() << QString("Configuration for named '%1' not exists").arg(beanName);
            continue;
        }
        auto metaObj = obj->metaObject();
        for (int i = 0; i < metaObj->methodCount(); ++i) {
            auto method = metaObj->method(i);
            QString tag = method.tag();
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
            auto tags = tag.split(' ', QString::SkipEmptyParts);
#else
            auto tags = tag.split(' ', Qt::SkipEmptyParts);
#endif
            bool isContained = false;
            for (auto t : tags) {
                if (t == MC_STRINGIFY(MC_BEAN)) {
                    isContained = true;
                    break;
                }
            }
            if (!isContained) {
                continue;
            }
            auto newBeanName = method.name();
            if (appCtx->isContained(newBeanName)) {
                qCritical() << QString(
                                   "want to inject bean '%1' is exists. the Configuration is '%2'")
                                   .arg(newBeanName, beanName);
                continue;
            }
            auto type = method.returnType();
            if (!sharedIds.contains(type)) {
                qCritical() << QString("return type is invalid of method '%1'. the bean is '%2'")
                                   .arg(newBeanName, beanName);
                continue;
            }
            QVector<QGenericArgument> arguments;
            arguments.resize(10);
            if (method.parameterCount() > arguments.length()) {
                qCritical()
                    << QString("argument length must less than 10 of method '%1'. the bean is '%2'")
                           .arg(method.name(), beanName);
                continue;
            }

            QVariant newBean(static_cast<QVariant::Type>(type));
            QGenericReturnArgument returnArg(method.typeName(), newBean.data());
            auto values = buildArguments(appCtx, method, beanName);
            for (int i = 0; i < values.size(); ++i) {
                arguments.replace(i, QGenericArgument("QVariant", values[i].data()));
            }
            method.invoke(obj.data(),
                          Qt::DirectConnection,
                          returnArg,
                          arguments.at(0),
                          arguments.at(1),
                          arguments.at(2),
                          arguments.at(3),
                          arguments.at(4),
                          arguments.at(5),
                          arguments.at(6),
                          arguments.at(7),
                          arguments.at(8),
                          arguments.at(9));
            McSimpleBeanDefinitionPtr beanDefinition = McSimpleBeanDefinitionPtr::create();
            beanDefinition->setBean(newBean);
            appCtx->registerBeanDefinition(newBeanName, beanDefinition);
        }
    }
}

QVector<QVariant> McConfigurationContainer::buildArguments(IMcApplicationContextConstPtrRef appCtx,
                                                           const QMetaMethod &method,
                                                           const QString &beanName) const noexcept
{
    auto sharedIds = McMetaTypeId::sharedPointerIds();
    QVector<QVariant> arguments;
    auto paramTypes = method.parameterTypes();
    auto paramNames = method.parameterNames();
    for (int i = 0; i < method.parameterCount(); ++i) {
        auto paramTypeName = paramTypes.at(i);
        auto paramType = method.parameterType(i);
        auto paramName = paramNames.at(i);
        if (!sharedIds.contains(paramType)) {
            qCritical()
                << QString("argument type named '%1' is invalid of method '%2'. the bean is '%3'")
                       .arg(paramTypeName, method.name(), beanName);
            return arguments;
        }
        auto bean = appCtx->getBeanToVariant(paramName);
        arguments.append(bean);
    }
    return arguments;
}
