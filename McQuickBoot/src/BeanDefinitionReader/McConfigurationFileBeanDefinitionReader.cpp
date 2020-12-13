#include "McBoot/BeanDefinitionReader/impl/McConfigurationFileBeanDefinitionReader.h"

#include <QCoreApplication>
#include <QMetaObject>
#include <QMetaProperty>
#include <QTemporaryFile>
#include <QSettings>
#include <QDebug>

#include <McIoc/Utils/McYaml.h>

#include <McIoc/ApplicationContext/IMcApplicationContext.h>
#include <McIoc/BeanDefinition/impl/McRootBeanDefinition.h>
#include <McIoc/ApplicationContext/impl/McSettingApplicationContext.h>
#include <McIoc/BeanFactory/impl/McMetaTypeId.h>

#include "McBoot/McBootGlobal.h"

MC_DECL_PRIVATE_DATA(McConfigurationFileBeanDefinitionReader)
IMcApplicationContextPtr appCtx;
MC_DECL_PRIVATE_DATA_END

McConfigurationFileBeanDefinitionReader::McConfigurationFileBeanDefinitionReader(
        IMcApplicationContextConstPtrRef appCtx
        , QObject *parent)
    : McAbstractBeanDefinitionReader(parent)
{
    MC_NEW_PRIVATE_DATA(McConfigurationFileBeanDefinitionReader);
    
    d->appCtx = appCtx;
}

McConfigurationFileBeanDefinitionReader::~McConfigurationFileBeanDefinitionReader() 
{
}

void McConfigurationFileBeanDefinitionReader::doReadBeanDefinition() noexcept 
{
    QString tempPath;
    {
        QTemporaryFile tempFile;
        if(!tempFile.open()) {
            qCritical("cannot open temporary file: %s\n", qPrintable(tempFile.fileName()));
            return;
        }
        tempFile.setAutoRemove(false);
        tempPath = tempFile.fileName();
    }
    QSettingsPtr setting = QSettingsPtr::create(tempPath, McYaml::format());
    QString defaultConfigPath = getDefaultConfigPath();
    auto beanDefinitions = d->appCtx->getBeanDefinitions();
    for(auto key : beanDefinitions.keys()) {
        auto value = beanDefinitions.value(key);
        auto metaObj = value->getBeanMetaObject();
        Q_ASSERT(metaObj != nullptr);
        auto index = metaObj->indexOfClassInfo(MC_CONFIGURATION_PROPERTIES_TAG);
        if(index == -1) {
            continue;
        }
        d->appCtx->unregisterBeanDefinition(key);
        auto pathIndex = metaObj->indexOfClassInfo(MC_PROPERTY_SOURCE_TAG);
        QString configPath;
        if(pathIndex == -1) {
            configPath = defaultConfigPath;
        } else {
            auto classInfo = metaObj->classInfo(pathIndex);
            configPath = classInfo.value();
        }
        if(!QFile::exists(configPath)) {
            qCritical("file not exist of default or specific for class: %s\n"
                      , metaObj->className());
            continue;
        }
        QString prefix = metaObj->classInfo(index).value();
        QString beanName;
        auto beanNameIndex = metaObj->indexOfClassInfo(MC_BEANNAME_TAG);
        if(beanNameIndex != -1) {
            beanName = metaObj->classInfo(beanNameIndex).value();
        } else {
            beanName = prefix;
        }
        setting->beginGroup(beanName);
        copyBeanDefinition(setting, configPath, prefix, metaObj);
        setting->endGroup();
    }
    
    auto appCtx = McSettingApplicationContextPtr::create(setting);
    auto otherBeanDefinitions = appCtx->getBeanDefinitions();
    for(auto key : otherBeanDefinitions.keys()) {
        auto value = otherBeanDefinitions.value(key);
        d->appCtx->registerBeanDefinition(key, value);
    }
    
    appCtx.reset();
    setting.reset();
    QFile::remove(tempPath);
}

QString McConfigurationFileBeanDefinitionReader::getDefaultConfigPath() const noexcept
{
    QString fileName = "application.yml";
    QString path = Mc::toAbsolutePath("./config/" + fileName);
    if(QFile::exists(path)) {
        return path;
    }
    return Mc::toAbsolutePath("./" + fileName);
}

void McConfigurationFileBeanDefinitionReader::copyBeanDefinition(
        QSettingsConstPtrRef setting, const QString &path
        , const QString &prefix, const QMetaObject *metaObj) noexcept
{
    QSettings originSet(path, McYaml::format());
    if(originSet.status() != QSettings::NoError) {
        qCritical() << "failed parse yaml file:" << path
                    << ". the status:" << originSet.status();
        return;
    }
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    QStringList groups = prefix.split(".", QString::SkipEmptyParts);
#else
    QStringList groups = prefix.split(".", Qt::SkipEmptyParts);
#endif
    for(auto group : groups) {
        originSet.beginGroup(group);
    }
    
    copyBeanDefinition(setting, originSet, metaObj);
    setting->setValue(MC_QSETTING_CLASS, metaObj->className());
    
    for(auto group : groups) {
        Q_UNUSED(group)
        originSet.endGroup();
    }
}

void McConfigurationFileBeanDefinitionReader::copyBeanDefinition(
        QSettingsConstPtrRef setting, QSettings &originSet, const QMetaObject *metaObj) noexcept
{
    auto childKeys = originSet.childKeys();
    for(auto childKey : childKeys) {
        auto value = originSet.value(childKey);
        if(metaObj == nullptr) {
            setting->setValue(childKey, value);
            continue;
        }
        value = buildChildProperty(childKey, value, metaObj);
        setting->setValue(childKey, value);
    }
    
    auto childGroups = originSet.childGroups();
    for(auto childGroup : childGroups) {
        setting->beginGroup(childGroup);
        originSet.beginGroup(childGroup);
        auto proIndex = metaObj->indexOfProperty(qPrintable(childGroup));
        if(proIndex == -1) {
            copyBeanDefinition(setting, originSet, nullptr);
        } else {
            auto childProType = QMetaType::type(metaObj->property(proIndex).typeName());
            auto sharedIds = McMetaTypeId::sharedPointerIds();
            if(!sharedIds.contains(childProType)) {
                copyBeanDefinition(setting, originSet, nullptr);
                continue;
            }
            auto childProId = sharedIds.value(childProType)->qobjectPointerId;
            auto childMetaObj = QMetaType::metaObjectForType(childProId);
            copyBeanDefinition(setting, originSet, childMetaObj);
            setting->setValue(MC_QSETTING_CLASS, childMetaObj->className());
        }
        originSet.endGroup();
        setting->endGroup();
    }
}

QVariant McConfigurationFileBeanDefinitionReader::buildChildProperty(
        const QString &proName, const QVariant &value, const QMetaObject *metaObj) noexcept
{
    auto proIndex = metaObj->indexOfProperty(qPrintable(proName));
    if(proIndex == -1) {
        return value;
    }
    
    if(!value.canConvert<QVariantList>()) {
        return value;
    }
    auto varList = value.toList();
    if(varList.isEmpty()) { //! 原则上来说这里永远不会进入
        return value;
    }
    auto var = varList.first();
    if(var.canConvert<QPair<QString, QVariant>>()) {
        auto pair = var.value<QPair<QString, QVariant>>();
        if(!pair.second.canConvert<QVariantMap>()) {
            return value;
        }
        auto mapType = QMetaType::type(metaObj->property(proIndex).typeName());
        auto assIds = McMetaTypeId::associativeIds();
        if(!assIds.contains(mapType)) {
            return value;
        }
        auto assId = assIds.value(mapType);
        auto childMetaObj = QMetaType::metaObjectForType(assId->valueId);
        for(auto &v : varList) {
            auto p = v.value<QPair<QString, QVariant>>();
            auto childProVar = p.second.value<QVariantMap>();
            for(auto childKey : childProVar.keys()) {
                childProVar[childKey] = buildChildProperty(childKey, childProVar.value(childKey), childMetaObj);
            }
            childProVar.insert(MC_QSETTING_CLASS, childMetaObj->className());
            p.second = childProVar;
            v = QVariant::fromValue(p);
        }
        return varList;
    }
    if(var.canConvert<QVariantMap>()) {
        auto listType = QMetaType::type(metaObj->property(proIndex).typeName());
        auto seqIds = McMetaTypeId::sequentialIds();
        if(!seqIds.contains(listType)) {
            return value;
        }
        auto seqId = seqIds.value(listType);
        auto childMetaObj = QMetaType::metaObjectForType(seqId->valueId);
        for(auto &v : varList) {
            auto map = v.value<QVariantMap>();
            for(auto childKey : map.keys()) {
                map[childKey] = buildChildProperty(childKey, map.value(childKey), childMetaObj);
            }
            map.insert(MC_QSETTING_CLASS, childMetaObj->className());
            v = map;
        }
        return varList;
    }
    return value;
}
