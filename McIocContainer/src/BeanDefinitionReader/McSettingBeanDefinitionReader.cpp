#include "McIoc/BeanDefinitionReader/impl/McSettingBeanDefinitionReader.h"

#include <QSettings>
#include <QLibrary>
#include <QFileInfo>
#include <QDebug>

#include "McIoc/BeanFactory/IMcBeanDefinitionRegistry.h"
#include "McIoc/BeanDefinition/impl/McRootBeanDefinition.h"
#include "McIoc/BeanFactory/impl/McBeanReference.h"
#include "McIoc/BeanFactory/impl/McBeanConnector.h"

MC_DECL_PRIVATE_DATA(McSettingBeanDefinitionReader)
QList<QSettingsPtr> settings;
MC_DECL_PRIVATE_DATA_END

McSettingBeanDefinitionReader::McSettingBeanDefinitionReader(
        QSettingsConstPtrRef setting
        , QObject *parent)
    : McSettingBeanDefinitionReader(QList<QSettingsPtr>() << setting, parent)
{
}

McSettingBeanDefinitionReader::McSettingBeanDefinitionReader(
        const QList<QSettingsPtr> &settings
        , QObject *parent)
    : McAbstractBeanDefinitionReader(parent)
{
    MC_NEW_PRIVATE_DATA(McSettingBeanDefinitionReader);
    
    d->settings = settings;
}

McSettingBeanDefinitionReader::~McSettingBeanDefinitionReader()
{
}

void McSettingBeanDefinitionReader::doReadBeanDefinition() noexcept 
{
    for(auto s : d->settings) {
        auto beanNames = s->childGroups();
        for(auto beanName : beanNames) {
            s->beginGroup(beanName);
            auto beanDefinition = buildBeanDefinition(s);
            if(beanDefinition.isNull()) {
                qCritical("build '%s' occurred error", qPrintable(beanName));
                continue;
            }
            //! 向注册容器 添加bean名称和bean定义. 如果存在则替换
            registry()->registerBeanDefinition(beanName, beanDefinition);
            s->endGroup();
        }
    }
}

IMcBeanDefinitionPtr McSettingBeanDefinitionReader::buildBeanDefinition(QSettingsConstPtrRef setting) noexcept
{
    McRootBeanDefinitionPtr beanDefinition = McRootBeanDefinitionPtr::create();
    bool isSingleton = setting->value(MC_QSETTING_SINGLETON, true).toBool();
    beanDefinition->setSingleton(isSingleton);
    if(setting->contains(MC_QSETTING_CLASS)) {
        beanDefinition->setClassName(setting->value(MC_QSETTING_CLASS).toString());
    } else if(setting->contains(MC_QSETTING_PLUGIN)) {
        auto pluginPath = setting->value(MC_QSETTING_PLUGIN).toString();
        pluginPath = Mc::toAbsolutePath(pluginPath);
        if(!QLibrary::isLibrary(pluginPath)){
            qCritical() << pluginPath << "is not a plugin. please check!!!";
            return IMcBeanDefinitionPtr();
        }
        beanDefinition->setPluginPath(pluginPath);
        beanDefinition->setSingleton(true);     //!< 插件必须是单例
    } else {
        qCritical("you must be set '%s' or '%s'", MC_QSETTING_CLASS, MC_QSETTING_PLUGIN);
        return IMcBeanDefinitionPtr();
    }
    readBeanDefinitionForProperty(setting, beanDefinition);
    readBeanDefinitionForConnect(setting, beanDefinition);
    return beanDefinition;
}

void McSettingBeanDefinitionReader::readBeanDefinitionForProperty(
        QSettingsConstPtrRef setting
        , IMcBeanDefinitionConstPtrRef beanDefinition) noexcept
{
    auto pros = setting->childKeys();
    pros.removeAll(MC_QSETTING_CLASS);
    pros.removeAll(MC_QSETTING_PLUGIN);
    pros.removeAll(MC_QSETTING_SINGLETON);
    pros.removeAll(MC_QSETTING_CONNECTS);
    for(auto pro : pros) {
        auto value = parseProperty(setting->value(pro));
        beanDefinition->addProperty(pro, value);
    }
    auto childBeanNames = setting->childGroups();
    for(auto childBeanName : childBeanNames) {
        setting->beginGroup(childBeanName);
        auto childBeanDefinition = buildBeanDefinition(setting);
        if(childBeanDefinition.isNull()) {
            qCritical("build '%s' occurred error", qPrintable(childBeanName));
            continue;
        }
        childBeanDefinition->setSingleton(true);
        QString beanName;
        QString className = beanDefinition->getClassName();
        if(className.isEmpty()) {
            QFileInfo fileInfo(beanDefinition->getPluginPath());
            className = fileInfo.baseName();
        }
        beanName = QString("__mc__%1_%2").arg(className, childBeanName);
        auto tmpBeanName = beanName;
        int index = 1;
        while(registry()->isContained(tmpBeanName)) {
            tmpBeanName = beanName;
            tmpBeanName.append('_');
            tmpBeanName.append(QString::number(index++));
        }
        beanName = tmpBeanName;
        //! 向注册容器 添加bean名称和bean定义. 如果存在则替换
        registry()->registerBeanDefinition(beanName, childBeanDefinition);
        McBeanReferencePtr ref = McBeanReferencePtr::create();
        ref->setName(beanName);
        beanDefinition->addProperty(childBeanName, QVariant::fromValue(ref));
        setting->endGroup();
    }
}

void McSettingBeanDefinitionReader::readBeanDefinitionForConnect(
        QSettingsConstPtrRef setting
        , IMcBeanDefinitionConstPtrRef beanDefinition) noexcept
{
    if(!setting->contains(MC_QSETTING_CONNECTS)) {
        return;
    }
    auto cs = setting->value(MC_QSETTING_CONNECTS).value<QList<QVariantMap>>();
    for(auto c : cs) {
        if(!c.contains(MC_QSETTING_SINGAL)
                || !c.contains(MC_QSETTING_SLOT)) {
            continue;
        }
        McBeanConnectorPtr connector = McBeanConnectorPtr::create();
        connector->setSender(c.value(MC_QSETTING_SENDER, "this").toString());
        connector->setSignal(c.value(MC_QSETTING_SINGAL).toString());
        connector->setReceiver(c.value(MC_QSETTING_RECEIVER, "this").toString());
        connector->setSlot(c.value(MC_QSETTING_SLOT).toString());
        auto type = c.value(MC_QSETTING_TYPE, "AutoConnection").toString();
        connector->setType(getConnectionType(type));
        QVariant var;
        var.setValue(connector);
        beanDefinition->addConnector(var);
    }
}

QVariant McSettingBeanDefinitionReader::parseProperty(const QVariant &var) noexcept
{
    if(var.canConvert<QString>()) {
        bool ok = false;
        auto i = var.toInt(&ok);
        if(ok) {
            return i;
        }
        auto value = var.toString();
        if(value.startsWith(MC_QSETTING_REF_TAG)) {
            McBeanReferencePtr ref = McBeanReferencePtr::create();
            ref->setName(value.remove(0, 1));
            return QVariant::fromValue(ref);
        } else {
            return var;
        }
    }
    if(var.canConvert<QVariantList>()) {
        auto value = var.value<QVariantList>();
        QVariantList list;
        QMap<QVariant, QVariant> map;
        for(auto v : value) {
            auto parseValue = parseProperty(v);
            if(parseValue.canConvert<QPair<QString, QVariant>>()) {
                auto pair = parseValue.value<QPair<QString, QVariant>>();
                map.insert(pair.first, pair.second);
            } else {
                list << parseValue;
            }
        }
        if(!list.isEmpty()) {
            return list;
        }
        if(!map.isEmpty()) {
            return QVariant::fromValue(map);
        }
        return QVariant();
    }
    if(var.canConvert<QPair<QString, QVariant>>()) {
        auto pair = var.value<QPair<QString, QVariant>>();
        pair.second = parseProperty(pair.second);
        return QVariant::fromValue(pair);
    }
    return QVariant();
}
