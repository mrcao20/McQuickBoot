#include "McYaml/McYaml.h"

#include <QIODevice>

namespace McPrivate {

void readYamlFile_helper(const QString &key, const YAML::Node &node, QSettings::SettingsMap &map)
{
    if(node.IsNull() || !node.IsDefined()) {
        map.insert(key, QVariant());
        return;
    }
    if(node.IsScalar() || node.IsSequence()) {
        map.insert(key, node.as<QVariant>());
        return;
    }
    for(auto itr = node.begin(), end = node.end(); itr != end; ++itr) {
        YAML::Node keyNode = itr->first;
        YAML::Node valueNode = itr->second;
        QString keyStr = key + "/" + keyNode.as<QString>();
        if(keyStr.startsWith("/")) {
            keyStr.remove(0, 1);
        }
        readYamlFile_helper(keyStr, valueNode, map);
    }
}

bool readYamlFile(QIODevice &device, QSettings::SettingsMap &map)
{
    try {
        auto root = YAML::Load(device.readAll());
        if(!root.IsMap()) {
            Q_ASSERT_X(false, "load yaml data", "root node must be key-value type");
            return false;
        }
        readYamlFile_helper("", root, map);
        return true;
    } catch (const std::exception &e) {
        qCritical("parse yaml failure. exception: ", e.what());
        return false;
    }
}

void writeYamlFile_helper(QVariantMap &map, const QStringList &keyList, const QVariant &value)
{
    if(keyList.isEmpty()) {
        return;
    }
    if(keyList.length() == 1) {
        map[keyList.first()] = value;
        return;
    }
    QVariantMap childMap;
    QVariant &existVar = map[keyList.first()];
    if(static_cast<QMetaType::Type>(existVar.type()) == QMetaType::QVariantMap) {
        childMap = existVar.toMap();
    }
    writeYamlFile_helper(childMap, keyList.mid(1), value);
    map[keyList.first()] = childMap;
}

bool writeYamlFile(QIODevice &device, const QSettings::SettingsMap &map)
{
    try {
        YAML::Node node(YAML::NodeType::Map);
        QVariantMap resultMap;
        for(auto key : map.keys()) {
            auto value = map.value(key);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
            QStringList keyList = key.split("/", Qt::SkipEmptyParts);
#else
            QStringList keyList = key.split("/", QString::SkipEmptyParts);
#endif
            writeYamlFile_helper(resultMap, keyList, value);
        }
        for(auto key : resultMap.keys()) {
            node.force_insert(key, resultMap.value(key));
        }
        device.write(QByteArray::fromStdString(YAML::Dump(node)));
        return true;
    } catch (const std::exception &e) {
        qCritical("parse yaml failure. exception: ", e.what());
        return false;
    }
}

}

QSettings::Format McYaml::m_format = QSettings::InvalidFormat;

void McYaml::registerYaml() noexcept
{
    m_format = QSettings::registerFormat("yml", McPrivate::readYamlFile
                                         , McPrivate::writeYamlFile);
}
