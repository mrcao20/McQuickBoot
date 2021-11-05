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
#include "McIoc/Utils/McYaml.h"

#include <QCoreApplication>
#include <QIODevice>
#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtCore/QList>
#include <QtCore/QPair>

#include <yaml-cpp/yaml.h>

namespace YAML {

// QString
template<>
struct convert<QString>
{
    static Node encode(const QString &rhs)
    {
        return Node(rhs.toStdString());
    }

    static bool decode(const Node &node, QString &rhs)
    {
        if (!node.IsScalar())
            return false;
        rhs = QString::fromStdString(node.Scalar());
        return true;
    }
};

// QVariant
template<>
struct convert<QVariant>
{
    static Node encode(const QVariant& rhs)
    {
        if(rhs.canConvert<QVariantMap>()) {
            QVariantMap map = rhs.toMap();
            if(map.size() == 1) {
                auto pair = qMakePair(map.firstKey(), map.first());
                return Node(pair);
            } else {
                return Node(map);
            }
        } else if(rhs.canConvert<QVariantList>()) {
            return Node(rhs.toList());
        } else if(rhs.canConvert<QPair<QString, QVariant>>()) {
            return Node(rhs.value<QPair<QString, QVariant>>());
        } else {
            return Node(rhs.toString());
        }
    }

    static bool decode(const Node &node, QVariant &rhs)
    {
        if (!node.IsDefined() || node.IsNull()) {
            return false;
        }
        if (node.IsScalar()) {
            rhs = node.as<QString>();
        } else if (node.IsSequence()) {
            rhs = node.as<QVariantList>();
        } else if (node.IsMap()) {
            auto map = node.as<QVariantMap>();
            if(map.size() == 1) {
                auto pair = qMakePair(map.firstKey(), map.first());
                rhs.setValue(pair);
            } else {
                rhs = map;
            }
        }
        return true;
    }
};

// QMap
template<typename Key, typename Value>
struct convert<QMap<Key, Value> >
{
    static Node encode(const QMap<Key,Value> &rhs)
    {
        Node node(NodeType::Map);
        auto it = rhs.constBegin();
        while (it != rhs.constEnd()) {
            node.force_insert(it.key(), it.value());
            ++it;
        }
        return node;
    }

    static bool decode(const Node &node, QMap<Key, Value> &rhs)
    {
        if (!node.IsMap())
            return false;

        rhs.clear();
        const_iterator it = node.begin();
        while (it != node.end()) {
            rhs[it->first.as<Key>()] = it->second.as<Value>();
            ++it;
        }
        return true;
    }
};

// QList
template<typename T>
struct convert<QList<T> >
{
    static Node encode(const QList<T> &rhs)
    {
        Node node(NodeType::Sequence);
        for (const T &value : rhs) {
            node.push_back(value);
        }
        return node;
    }

    static bool decode(const Node &node, QList<T> &rhs)
    {
        if (!node.IsSequence())
            return false;

        rhs.clear();
        const_iterator it = node.begin();
        while (it != node.end()) {
            rhs.push_back(it->as<T>());
            ++it;
        }
        return true;
    }
};

// TODO: Add the rest of the container classes
// QLinkedList, QStack, QQueue, QSet, QMultiMap, QHash, QMultiHash, QStringList, ...

} // end namespace YAML

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
    if(device.bytesAvailable() == 0) {
        return true;
    }
    try {
        auto root = YAML::Load(device.readAll());
        if(!root.IsMap()) {
            Q_ASSERT_X(false, "load yaml data", "root node must be key-value type");
            return false;
        }
        readYamlFile_helper("", root, map);
        return true;
    } catch (const std::exception &e) {
        qCritical("parse yaml failure. exception: %s\n", e.what());
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
    if (existVar.typeId() == QMetaType::QVariantMap) {
        childMap = existVar.toMap();
    }
    writeYamlFile_helper(childMap, keyList.mid(1), value);
    map[keyList.first()] = childMap;
}

bool writeYamlFile(QIODevice &device, const QSettings::SettingsMap &map)
{
    if(map.isEmpty()) {
        return true;
    }
    try {
        YAML::Node node(YAML::NodeType::Map);
        QVariantMap resultMap;
        for(auto key : map.keys()) {
            auto value = map.value(key);
            QStringList keyList = key.split("/", Qt::SkipEmptyParts);
            writeYamlFile_helper(resultMap, keyList, value);
        }
        for(auto key : resultMap.keys()) {
            node.force_insert(key, resultMap.value(key));
        }
        device.write(QByteArray::fromStdString(YAML::Dump(node)));
        return true;
    } catch (const std::exception &e) {
        qCritical("parse yaml failure. exception: %s\n", e.what());
        return false;
    }
}

}

MC_INIT(McYaml)
m_format = QSettings::registerFormat("yml", McPrivate::readYamlFile
                                     , McPrivate::writeYamlFile);
MC_INIT_END

QSettings::Format McYaml::m_format = QSettings::InvalidFormat;
