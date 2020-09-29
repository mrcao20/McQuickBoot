#include "McBoot/BeanDefinitionReader/impl/McConfigurationFileBeanDefinitionReader.h"

#include <QCoreApplication>
#include <QMetaObject>
#include <QMetaProperty>
#include <QTemporaryFile>
#include <QSettings>
#include <QDebug>

#include <McYaml/McYaml.h>

#include <McIoc/ApplicationContext/IMcApplicationContext.h>
#include <McIoc/BeanDefinition/impl/McRootBeanDefinition.h>

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
        copyBeanDefinition(setting, configPath, prefix);
        setting->endGroup();
    }
    
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
        QSettingsConstPtrRef setting, const QString &path, const QString &prefix) noexcept
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
    
    copyBeanDefinition(setting, originSet);
    
    for(auto group : groups) {
        Q_UNUSED(group)
        originSet.endGroup();
    }
}

void McConfigurationFileBeanDefinitionReader::copyBeanDefinition(
        QSettingsConstPtrRef setting, QSettings &originSet) noexcept
{
    auto childKeys = originSet.childKeys();
    for(auto childKey : childKeys) {
        setting->setValue(childKey, originSet.value(childKey));
    }
    
    auto childGroups = originSet.childGroups();
    for(auto childGroup : childGroups) {
        setting->beginGroup(childGroup);
        originSet.beginGroup(childGroup);
        copyBeanDefinition(setting, originSet);
        originSet.endGroup();
        setting->endGroup();
    }
}
