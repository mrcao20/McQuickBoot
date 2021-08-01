#include "McBoot/Configuration/McConfigurationContainer.h"

#include <QDebug>
#include <QMetaClassInfo>

#include <McIoc/ApplicationContext/IMcApplicationContext.h>
#include <McIoc/BeanFactory/impl/McMetaTypeId.h>

#include "McBoot/Configuration/McSimpleBeanDefinition.h"
#include "McBoot/Configuration/Parser/McConfigurationParserFactory.h"
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
    QMap<int, QList<QVariant>> vars;
    auto appCtx = boot->getApplicationContext();
    auto beanNames = Mc::getComponents(appCtx, MC_CONFIGURATION_TAG);
    for (const auto &beanName : beanNames) {
        auto var = appCtx->getBeanToVariant(beanName);
        if (!var.isValid()) {
            qCritical() << QString("Configuration for named '%1' not exists").arg(beanName);
            continue;
        }
        QObject *obj = var.value<QObject *>();
        if (obj == nullptr) {
            obj = var.value<QObjectPtr>().data();
        }
        Q_ASSERT(obj != nullptr);
        auto metaObj = obj->metaObject();
        int priorityIndex = metaObj->indexOfClassInfo(MC_CONFIGURATION_PRIORITY_TAG);
        Q_ASSERT(priorityIndex != -1);
        auto classInfo = metaObj->classInfo(priorityIndex);
        QString priority = classInfo.value();
        Q_ASSERT(!priority.isEmpty());
        priority.remove(R"("@")");
        if (priority.isEmpty()) {
            priority = "0";
        }
        bool isOk = false;
        int prio = priority.toInt(&isOk);
        Q_ASSERT(isOk);
        vars[prio].append(var);
    }
    const auto &parsers = McConfigurationParserFactory::getParsers();
    auto keys = vars.keys();
    for (int i = keys.size() - 1; i >= 0; --i) {
        auto value = vars.value(keys.at(i));
        for (const auto &v : value) {
            for (const auto &parser : parsers) {
                parser->parse(v);
            }
            QObject *obj = v.value<QObject *>();
            if (obj == nullptr) {
                obj = v.value<QObjectPtr>().data();
            }
            Q_ASSERT(obj != nullptr);
            auto metaObj = obj->metaObject();
            for (int i = 0; i < metaObj->methodCount(); ++i) {
                auto method = metaObj->method(i);
                if (!Mc::isContainedTag(method.tag(), MC_STRINGIFY(MC_BEAN))) {
                    continue;
                }
                auto newBeanName = method.name();
                if (appCtx->isContained(newBeanName)) {
                    qCritical() << QString("want to inject bean '%1' is exists.")
                                       .arg(QString(newBeanName));
                    continue;
                }
                auto type = method.returnType();
                if (type == QMetaType::UnknownType) {
                    qCritical() << QString("return type is invalid of method '%1'.")
                                       .arg(QString(newBeanName));
                    continue;
                }
                QVector<QGenericArgument> arguments;
                arguments.resize(10);
                if (method.parameterCount() > arguments.length()) {
                    qCritical() << QString("argument length must less than 10 of method '%1'.")
                                       .arg(QString(method.name()));
                    continue;
                }

                QVariant newBean(static_cast<QVariant::Type>(type));
                QGenericReturnArgument returnArg(method.typeName(), newBean.data());
                auto values = buildArguments(appCtx, method);
                for (int i = 0; i < values.size(); ++i) {
                    arguments.replace(i, QGenericArgument("QVariant", values[i].data()));
                }
                method.invoke(obj,
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
}

QVector<QVariant> McConfigurationContainer::buildArguments(IMcApplicationContextConstPtrRef appCtx,
                                                           const QMetaMethod &method) const noexcept
{
    QVector<QVariant> arguments;
    auto paramTypes = method.parameterTypes();
    auto paramNames = method.parameterNames();
    for (int i = 0; i < method.parameterCount(); ++i) {
        auto paramTypeName = paramTypes.at(i);
        auto paramType = method.parameterType(i);
        auto paramName = paramNames.at(i);
        if (paramType == QMetaType::UnknownType) {
            qCritical() << QString("argument type named '%1' is invalid of method '%2'.")
                               .arg(QString(paramTypeName), QString(method.name()));
            return arguments;
        }
        auto bean = appCtx->getBeanToVariant(paramName);
        if (!bean.isValid()) {
            qCritical() << QString("cannot get bean '%1' from application context")
                               .arg(QString(paramName));
            return arguments;
        }
        arguments.append(bean);
    }
    return arguments;
}
