#include "McYaml/McYaml.h"

#include <QIODevice>

namespace McPrivate {

bool readYamlFile(QIODevice &device, QSettings::SettingsMap &map)
{
    try {
        auto root = YAML::Load(device.readAll());
        if(root.Type() != YAML::NodeType::Map) {
            Q_ASSERT_X(false, "load yaml data", "root node must be key-value type");
            return false;
        }
        map = root.as<QSettings::SettingsMap>();
        return true;
    } catch (const std::exception &e) {
        qCritical("parse yaml failure. exception: ", e.what());
        return false;
    }
}

bool writeYamlFile(QIODevice &device, const QSettings::SettingsMap &map)
{
    try {
        YAML::Node node(YAML::NodeType::Map);
        for(auto key : map.keys()) {
            node.force_insert(key, map.value(key));
        }
        device.write(QByteArray::fromStdString(YAML::Dump(node)));
        return true;
    } catch (const std::exception &e) {
        qCritical("parse yaml failure. exception: ", e.what());
        return false;
    }
}

}

void McYaml::registerYaml() noexcept
{
    m_format = QSettings::registerFormat("yaml", McPrivate::readYamlFile
                                         , McPrivate::writeYamlFile);
}
