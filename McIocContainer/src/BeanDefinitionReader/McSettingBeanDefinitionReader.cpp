#include "McIoc/BeanDefinitionReader/impl/McSettingBeanDefinitionReader.h"

#include <QSettings>
#include <QLibrary>
#include <QDebug>

#include "McIoc/BeanFactory/IMcBeanDefinitionRegistry.h"
#include "McIoc/BeanDefinition/impl/McRootBeanDefinition.h"
#include "McIoc/BeanFactory/impl/McBeanReference.h"
#include "McIoc/BeanFactory/impl/McBeanConnector.h"

MC_DECL_PRIVATE_DATA(McSettingBeanDefinitionReader)
QSettingsPtr settings;
MC_DECL_PRIVATE_DATA_END

McSettingBeanDefinitionReader::McSettingBeanDefinitionReader(
        QSettingsConstPtrRef settings
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
}

Qt::ConnectionType McSettingBeanDefinitionReader::getConnectionType(const QString &typeStr) noexcept 
{
    Qt::ConnectionType type = Qt::ConnectionType::AutoConnection;
    
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    QStringList typeList = typeStr.simplified().split('|', Qt::SkipEmptyParts);
#else
    QStringList typeList = typeStr.simplified().split('|', QString::SkipEmptyParts);
#endif
    
    if(typeList.size() > 2) {
        qCritical() << "请按照QObject::connect的要求设置ConnectionType";
        return type;
    }
    
    if(typeList.size() == 1) {
        auto temp = connectionTypeStrToEnum(typeList.at(0));
        if(temp == Qt::ConnectionType::UniqueConnection)
            type = static_cast<Qt::ConnectionType>(type | temp);
        else {
            type = temp;
        }
    } else if(typeList.size() == 2) {
        auto temp1 = connectionTypeStrToEnum(typeList.at(0));
        auto temp2 = connectionTypeStrToEnum(typeList.at(1));
        if(temp1 != Qt::ConnectionType::UniqueConnection
                && temp2 != Qt::ConnectionType::UniqueConnection) {
            qCritical() << "如果指定两种连接方式，则至少有一种必须为Qt::UniqueConnection";
        }else if(temp1 == Qt::ConnectionType::UniqueConnection
                 && temp2 == Qt::ConnectionType::UniqueConnection) {
            type = static_cast<Qt::ConnectionType>(type | temp1);
        }else {
            type = static_cast<Qt::ConnectionType>(temp1 | temp2);
        }
    }
    return type;
}

Qt::ConnectionType McSettingBeanDefinitionReader::connectionTypeStrToEnum(const QString &typeStr) noexcept 
{
    auto type = typeStr.simplified();
    if(type == "AutoConnection") {
        return Qt::ConnectionType::AutoConnection;
    }else if(type == "DirectConnection") {
        return Qt::ConnectionType::DirectConnection;
    }else if(type == "QueuedConnection") {
        return Qt::ConnectionType::QueuedConnection;
    }else if(type == "BlockingQueuedConnection") {
        return Qt::ConnectionType::BlockingQueuedConnection;
    }else {
        return Qt::ConnectionType::UniqueConnection;
    }
}
