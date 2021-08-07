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

MC_GLOBAL_STATIC_BEGIN(staticData)
QStringList configPaths;
MC_GLOBAL_STATIC_END(staticData)

MC_STATIC()
staticData->configPaths.append(QLatin1String("./config/application.yml"));
staticData->configPaths.append(QLatin1String("./application.yml"));
for (auto &path : staticData->configPaths) {
    path = Mc::toAbsolutePath(path);
}
MC_STATIC_END

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

void McConfigurationFileBeanDefinitionReader::addConfigPath(const QString &path) noexcept
{
    staticData->configPaths.append(path);
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
    auto keys = beanDefinitions.keys();
    for (auto &key : qAsConst(keys)) {
        auto value = beanDefinitions.value(key);
        auto metaObj = value->getBeanMetaObject();
        Q_ASSERT(metaObj != nullptr);
        auto index = metaObj->indexOfClassInfo(MC_CONFIGURATION_PROPERTIES_TAG);
        if(index == -1) {
            continue;
        }
        auto tmpBean = value->getBean();
        if (tmpBean.isValid()) {
            continue;
        }

        d->appCtx->unregisterBeanDefinition(key);
        auto pathIndex = metaObj->indexOfClassInfo(MC_PROPERTY_SOURCE_TAG);
        QString configPath;
        if(pathIndex == -1) {
            configPath = defaultConfigPath;
        } else {
            auto classInfo = metaObj->classInfo(pathIndex);
            configPath = Mc::toAbsolutePath(classInfo.value());
        }
        if(!QFile::exists(configPath)) {
            qCritical("file not exist of default or specific for class: %s\n", metaObj->className());
            continue;
        }
        QString prefix = metaObj->classInfo(index).value();
        QString beanName;
        auto beanNameIndex = metaObj->indexOfClassInfo(MC_BEANNAME_TAG);
        if(beanNameIndex != -1) {
            beanName = metaObj->classInfo(beanNameIndex).value();
            if (beanName.isEmpty()) {
                beanName = prefix;
            }
        } else {
            beanName = prefix;
        }
        setting->beginGroup(beanName);
        copyBeanDefinition(setting, configPath, prefix, metaObj);
        setting->endGroup();
    }

    auto appCtx = McSettingApplicationContextPtr::create(setting);
    auto otherBeanDefinitions = appCtx->getBeanDefinitions();
    keys = otherBeanDefinitions.keys();
    for (auto &key : qAsConst(keys)) {
        auto value = otherBeanDefinitions.value(key);
        d->appCtx->registerBeanDefinition(key, value);
    }

    appCtx.reset();
    setting.reset();
    QFile::remove(tempPath);
}

QString McConfigurationFileBeanDefinitionReader::getDefaultConfigPath() const noexcept
{
    if (staticData->configPaths.isEmpty()) {
        return "";
    }
    for (auto &p : qAsConst(staticData->configPaths)) {
        if (QFile::exists(p)) {
            return p;
        }
    }
    return staticData->configPaths.last();
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
    for (auto &group : qAsConst(groups)) {
        originSet.beginGroup(group);
    }

    copyBeanDefinition(setting, originSet, metaObj);
    setting->setValue(Mc::Constant::Tag::QSetting::clazz, metaObj->className());

    for (auto &group : qAsConst(groups)) {
        Q_UNUSED(group)
        originSet.endGroup();
    }
}

void McConfigurationFileBeanDefinitionReader::copyBeanDefinition(
        QSettingsConstPtrRef setting, QSettings &originSet, const QMetaObject *metaObj) noexcept
{
    auto childKeys = originSet.childKeys();
    for (auto &childKey : qAsConst(childKeys)) {
        auto value = originSet.value(childKey);
        if(metaObj == nullptr) {
            setting->setValue(childKey, value);
            continue;
        }
        value = buildChildProperty(childKey, value, metaObj);
        setting->setValue(childKey, value);
    }

    auto childGroups = originSet.childGroups();
    for (auto &childGroup : qAsConst(childGroups)) {
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
            setting->setValue(Mc::Constant::Tag::QSetting::clazz, childMetaObj->className());
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
        auto sharedMetaIds = McMetaTypeId::sharedPointerIds();
        const QMetaObject *childMetaObj = nullptr;
        if (sharedMetaIds.contains(assId->valueId)) {
            childMetaObj = QMetaType::metaObjectForType(
                sharedMetaIds.value(assId->valueId)->qobjectPointerId);
        } else {
            childMetaObj = QMetaType::metaObjectForType(assId->valueId);
        }
        for(auto &v : varList) {
            auto p = v.value<QPair<QString, QVariant>>();
            auto childProVar = p.second.value<QVariantMap>();
            auto keys = childProVar.keys();
            for (auto &childKey : qAsConst(keys)) {
                childProVar[childKey] = buildChildProperty(childKey, childProVar.value(childKey), childMetaObj);
            }
            childProVar.insert(Mc::Constant::Tag::QSetting::clazz, childMetaObj->className());
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
        auto sharedMetaIds = McMetaTypeId::sharedPointerIds();
        const QMetaObject *childMetaObj = nullptr;
        if (sharedMetaIds.contains(seqId->valueId)) {
            childMetaObj = QMetaType::metaObjectForType(
                sharedMetaIds.value(seqId->valueId)->qobjectPointerId);
        } else {
            childMetaObj = QMetaType::metaObjectForType(seqId->valueId);
        }
        for(auto &v : varList) {
            auto map = v.value<QVariantMap>();
            auto keys = map.keys();
            for (auto &childKey : qAsConst(keys)) {
                map[childKey] = buildChildProperty(childKey, map.value(childKey), childMetaObj);
            }
            map.insert(Mc::Constant::Tag::QSetting::clazz, childMetaObj->className());
            v = map;
        }
        return varList;
    }
    return value;
}
