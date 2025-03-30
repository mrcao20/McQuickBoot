/*
 * Copyright (c) 2021 mrcao20/mrcao20@163.com
 * McQuickBoot is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#include "McConfigNode.h"

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QPair>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QVector>

#include <yaml-cpp/yaml.h>

namespace YAML {
//! QString
template<>
struct convert<QString>
{
    static Node encode(const QString &rhs) { return Node(rhs.toStdString()); }

    static bool decode(const Node &node, QString &rhs)
    {
        if (!node.IsScalar())
            return false;
        rhs = QString::fromStdString(node.Scalar());
        return true;
    }
};

//! QVariant
template<>
struct convert<QVariant>
{
    static Node encode(const QVariant &rhs)
    {
        if (rhs.canConvert<QMap<QVariant, QVariant>>()) {
            auto map = rhs.value<QMap<QVariant, QVariant>>();
            if (map.size() == 1) {
                auto pair = qMakePair(map.firstKey(), map.first());
                return Node(pair);
            } else {
                return Node(map);
            }
        } else if (rhs.canConvert<QString>()) {
            return Node(rhs.toString());
        } else if (rhs.canConvert<QVariantList>()) {
            return Node(rhs.toList());
        } else if (rhs.canConvert<QPair<QVariant, QVariant>>()) {
            return Node(rhs.value<QPair<QVariant, QVariant>>());
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
            auto map = node.as<QMap<QVariant, QVariant>>();
            if (map.size() == 1) {
                auto pair = qMakePair(map.firstKey(), map.first());
                rhs.setValue(pair);
            } else {
                rhs.setValue(map);
            }
        }
        return true;
    }
};

//! QMap
template<typename Key, typename Value>
struct convert<QMap<Key, Value>>
{
    static Node encode(const QMap<Key, Value> &rhs)
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

#ifdef MC_USE_QT5
//! QVector
template<typename T>
struct convert<QVector<T>>
{
    static Node encode(const QVector<T> &rhs)
    {
        Node node(NodeType::Sequence);
        for (const T &value : rhs) {
            node.push_back(value);
        }
        return node;
    }

    static bool decode(const Node &node, QVector<T> &rhs)
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
#endif

//! QList
template<typename T>
struct convert<QList<T>>
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

//! std::pair<QVariant, QVariant>
template<>
struct convert<std::pair<QVariant, QVariant>>
{
    static Node encode(const std::pair<QVariant, QVariant> &rhs)
    {
        Node node(NodeType::Map);
        node.force_insert(rhs.first, rhs.second);
        return node;
    }

    static bool decode(const Node &node, std::pair<QVariant, QVariant> &rhs)
    {
        if (!node.IsMap())
            return false;

        const_iterator itr = node.begin();
        if (itr != node.end()) {
            rhs.first = itr->first.as<QVariant>();
            rhs.second = itr->second.as<QVariant>();
        }
        return true;
    }
};

#ifdef MC_USE_QT5
//! QPair
template<typename T, typename U>
struct convert<QPair<T, U>>
{
    static Node encode(const QPair<T, U> &rhs)
    {
        Node node(NodeType::Map);
        node.force_insert(rhs.first, rhs.second);
        return node;
    }

    static bool decode(const Node &node, QPair<T, U> &rhs)
    {
        if (!node.IsMap())
            return false;

        const_iterator itr = node.begin();
        if (itr != node.end()) {
            rhs.first = itr->first.as<T>();
            rhs.second = itr->second.as<U>();
        }
        return true;
    }
};
#endif

//! TODO: Add the rest of the container classes
//! QLinkedList, QStack, QQueue, QSet, QMultiMap, QHash, QMultiHash, QStringList, ...
} // end namespace YAML

static void writeYamlNode(const YAML::Node &node, const QString &path) noexcept
{
    if (path.isEmpty()) {
        return;
    }
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qCWarning(mcCore) << file.error() << ":" << file.errorString();
        return;
    }
    try {
        YAML::Emitter emitter;
        emitter << node;
        file.write(emitter.c_str(), emitter.size());
    } catch (const std::exception &e) {
        qCCritical(mcCore, "dump yaml failure. exception: %s", e.what());
    }
}

MC_DECL_PRIVATE_DATA2(McConfigNode)
{
    bool isAutoSync{true};
    QString configPath;
    YAML::Node yamlNode;

    std::function<void(const QString &)> syncFunc;
};

MC_DECL_PRIVATE_DATA2(McConfigNodeIterator)
{
    YAML::iterator yamlItr;
};

MC_DECL_PRIVATE_DATA2(McConfigNodeConstIterator)
{
    YAML::const_iterator yamlItr;
};

McConfigNode::McConfigNode() noexcept
{
    MC_NEW_PRIVATE_DATA(McConfigNode);
}

McConfigNode::McConfigNode(NodeType type) noexcept
    : McConfigNode()
{
    try {
        auto yamlType = static_cast<YAML::NodeType::value>(type);
        if (type == NodeType::Pair) {
            yamlType = YAML::NodeType::Map;
        }
        d->yamlNode = YAML::Node(yamlType);
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
    }
}

McConfigNode::McConfigNode(const YAML::Node &yamlNode) noexcept
    : McConfigNode()
{
    d->yamlNode = yamlNode;
}

McConfigNode::McConfigNode(const QVariant &rhs) noexcept
    : McConfigNode()
{
    try {
        d->yamlNode = YAML::Node(rhs);
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
    }
}

McConfigNode::~McConfigNode() {}

McConfigNode::McConfigNode(const McConfigNode &o) noexcept
    : McConfigNode(o.d->yamlNode)
{
    d->isAutoSync = o.d->isAutoSync;
    d->configPath = o.d->configPath;
    d->syncFunc = o.d->syncFunc;
}

McConfigNode &McConfigNode::operator=(const McConfigNode &o) noexcept
{
    d->isAutoSync = o.d->isAutoSync;
    d->yamlNode = o.d->yamlNode;
    d->configPath = o.d->configPath;
    d->syncFunc = o.d->syncFunc;
    return *this;
}

McConfigNode::McConfigNode(McConfigNode &&o) noexcept
    : McConfigNode(o.d->yamlNode)
{
    d->isAutoSync = o.d->isAutoSync;
    d->configPath = o.d->configPath;
    d->syncFunc = o.d->syncFunc;

    o.d.reset();
}

McConfigNode &McConfigNode::operator=(McConfigNode &&o) noexcept
{
    d->isAutoSync = o.d->isAutoSync;
    d->yamlNode = o.d->yamlNode;
    d->configPath = o.d->configPath;
    d->syncFunc = o.d->syncFunc;

    o.d.reset();
    return *this;
}

McConfigNode McConfigNode::loadOrCreate(const QString &configPath, QIODevice::OpenMode flags) noexcept
{
    QString realPath = configPath;
    if (realPath.isEmpty()) {
        realPath = QStringLiteral("./config/runtimeConfig.yml");
    }
    realPath = Mc::toAbsolutePath(realPath);
    if (!QFile::exists(realPath)) {
        QFileInfo fileInfo(realPath);
        auto dir = fileInfo.absoluteDir();
        if (!dir.exists() && !dir.mkpath(dir.absolutePath())) {
            qCCritical(mcCore) << "cannot create config path";
            return McConfigNode();
        }
    }
    QFile file(realPath);
    if (!file.open(flags)) {
        qCCritical(mcCore) << "cannot open config file. error code:" << file.error()
                           << ". error:" << file.errorString();
        return McConfigNode();
    }
    try {
        McConfigNode node(YAML::Load(file.readAll()));
        if (flags.testFlag(QIODevice::WriteOnly)) {
            node.d->configPath = realPath;
            node.setSyncFunc();
        }
        return node;
    } catch (const std::exception &e) {
        qCCritical(mcCore, "parse yaml failure. exception: %s", e.what());
        return McConfigNode();
    }
}

bool McConfigNode::isAutoSync() const noexcept
{
    return d->isAutoSync;
}

void McConfigNode::setAutoSync(bool val) noexcept
{
    d->isAutoSync = val;
}

void McConfigNode::sync() noexcept
{
    syncTo(d->configPath);
}

void McConfigNode::syncTo(const QString &path) const noexcept
{
    if (d->syncFunc == nullptr)
        return;

    d->syncFunc(path);
}

void McConfigNode::syncCurrentTo(const QString &path) const noexcept
{
    writeYamlNode(d->yamlNode, path);
}

QString McConfigNode::configPath() const noexcept
{
    return d->configPath;
}

void McConfigNode::copyContentTo(McConfigNode &other) const noexcept
{
    other.d->yamlNode = YAML::Clone(d->yamlNode);
    if (other.d->syncFunc != nullptr) {
        other.setSyncFunc();
    }
}

McConfigNode::NodeType McConfigNode::type() const noexcept
{
    try {
        auto yamlType = d->yamlNode.Type();
        if (yamlType == YAML::NodeType::Map && d->yamlNode.size() == 1) {
            return NodeType::Pair;
        } else {
            return static_cast<McConfigNode::NodeType>(yamlType);
        }
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
        return McConfigNode::NodeType::Undefined;
    }
}

bool McConfigNode::isDefined() const noexcept
{
    try {
        return d->yamlNode.IsDefined();
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
        return false;
    }
}

QVariant McConfigNode::as() const noexcept
{
    try {
        return d->yamlNode.as<QVariant>();
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
        return QVariant();
    }
}

QVariant McConfigNode::as(const QVariant &fallback) const noexcept
{
    try {
        return d->yamlNode.as<QVariant>(fallback);
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
        return QVariant();
    }
}

QVariant McConfigNode::asMap() const noexcept
{
    try {
        return QVariant::fromValue(d->yamlNode.as<QMap<QVariant, QVariant>>());
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
        return QVariant();
    }
}

QVariant McConfigNode::asMap(const QVariant &fallback) const noexcept
{
    try {
        return QVariant::fromValue(d->yamlNode.as<QMap<QVariant, QVariant>>(fallback.value<QMap<QVariant, QVariant>>()));
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
        return QVariant();
    }
}

QVariant McConfigNode::asPair() const noexcept
{
    try {
        return QVariant::fromValue(d->yamlNode.as<QPair<QVariant, QVariant>>());
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
        return QVariant();
    }
}

QVariant McConfigNode::asPair(const QVariant &fallback) const noexcept
{
    try {
        return QVariant::fromValue(
            d->yamlNode.as<QPair<QVariant, QVariant>>(fallback.value<QPair<QVariant, QVariant>>()));
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
        return QVariant();
    }
}

QString McConfigNode::scalar() const noexcept
{
    try {
        return QString::fromStdString(d->yamlNode.Scalar());
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
        return QString();
    }
}

bool McConfigNode::is(const McConfigNode &rhs) const noexcept
{
    try {
        return d->yamlNode.is(rhs.d->yamlNode);
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
        return false;
    }
}

McConfigNode &McConfigNode::operator=(const QVariant &rhs) noexcept
{
    try {
        d->yamlNode = rhs;
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
    }
    sync_helper();
    return *this;
}

void McConfigNode::reset(const McConfigNode &rhs) noexcept
{
    try {
        d->yamlNode.reset(rhs.d->yamlNode);
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
    }
    if (d->syncFunc != nullptr) {
        setSyncFunc();
    }
    sync_helper();
}

std::size_t McConfigNode::size() const noexcept
{
    try {
        return d->yamlNode.size();
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
        return 0;
    }
}

McConfigNode McConfigNode::at(std::size_t index) const noexcept
{
    McConfigNode node;
    try {
        std::size_t tmp = 0;
        for (auto itr = d->yamlNode.begin(), end = d->yamlNode.end(); itr != end; ++itr) {
            if (tmp < index) {
                ++tmp;
                continue;
            }
            if (itr->IsDefined()) {
                node = McConfigNode(itr->as<YAML::Node>());
            } else {
                node[McConfigNode(itr->first)] = McConfigNode(itr->second);
            }
            break;
        }
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
    }
    return node;
}

QStringList McConfigNode::keys() const noexcept
{
    QStringList nodeKeys;
    try {
        for (auto itr = d->yamlNode.begin(), end = d->yamlNode.end(); itr != end; ++itr) {
            nodeKeys.append(itr->first.as<QString>());
        }
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
    }
    return nodeKeys;
}

QString McConfigNode::key() const noexcept
{
    if (!isPair()) {
        return QString();
    }
    try {
        return d->yamlNode.begin()->first.as<QString>();
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
        return QString();
    }
}

McConfigNode McConfigNode::value() const noexcept
{
    if (!isPair()) {
        return McConfigNode();
    }
    try {
        return McConfigNode(d->yamlNode.begin()->second);
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
        return McConfigNode();
    }
}

McConfigNode::iterator::iterator() noexcept
{
    MC_NEW_PRIVATE_DATA(McConfigNodeIterator);
}

McConfigNode::iterator::iterator(const YAML::iterator &yamlItr) noexcept
    : iterator()
{
    d->yamlItr = yamlItr;
}

McConfigNode::iterator::~iterator() {}

McConfigNode::iterator::iterator(const iterator &o) noexcept
    : iterator(o.d->yamlItr)
{
}

McConfigNode::iterator &McConfigNode::iterator::operator=(const iterator &o) noexcept
{
    d->yamlItr = o.d->yamlItr;
    return *this;
}

McConfigNode::iterator::iterator(iterator &&o) noexcept
    : iterator(o.d->yamlItr)
{
    o.d.reset();
}

McConfigNode::iterator &McConfigNode::iterator::operator=(iterator &&o) noexcept
{
    d->yamlItr = o.d->yamlItr;

    o.d.reset();
    return *this;
}

McConfigNode::iterator &McConfigNode::iterator::operator++()
{
    ++d->yamlItr;
    return *this;
}

McConfigNode::iterator McConfigNode::iterator::operator++(int)
{
    iterator tmp(d->yamlItr);
    ++(*this);
    return tmp;
}

bool McConfigNode::iterator::operator==(const iterator &rhs) const
{
    return d->yamlItr == rhs.d->yamlItr;
}

bool McConfigNode::iterator::operator!=(const iterator &rhs) const
{
    return d->yamlItr != rhs.d->yamlItr;
}

McConfigNode McConfigNode::iterator::operator*() const
{
    McConfigNode node;
    try {
        if (d->yamlItr->IsDefined()) {
            node = McConfigNode(d->yamlItr->as<YAML::Node>());
        } else {
            node[McConfigNode(d->yamlItr->first)] = McConfigNode(d->yamlItr->second);
        }
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
    }
    return node;
}

// McConfigNode::iterator::Proxy McConfigNode::iterator::operator->() const
//{
//     return Proxy(operator*());
// }

McConfigNode::const_iterator::const_iterator() noexcept
{
    MC_NEW_PRIVATE_DATA(McConfigNodeConstIterator);
}

McConfigNode::const_iterator::const_iterator(const YAML::const_iterator &yamlItr) noexcept
    : const_iterator()
{
    d->yamlItr = yamlItr;
}

McConfigNode::const_iterator::~const_iterator() {}

McConfigNode::const_iterator::const_iterator(const const_iterator &o) noexcept
    : const_iterator(o.d->yamlItr)
{
}

McConfigNode::const_iterator &McConfigNode::const_iterator::operator=(const const_iterator &o) noexcept
{
    d->yamlItr = o.d->yamlItr;
    return *this;
}

McConfigNode::const_iterator::const_iterator(const_iterator &&o) noexcept
    : const_iterator(o.d->yamlItr)
{
    o.d.reset();
}

McConfigNode::const_iterator &McConfigNode::const_iterator::operator=(const_iterator &&o) noexcept
{
    d->yamlItr = o.d->yamlItr;

    o.d.reset();
    return *this;
}

McConfigNode::const_iterator &McConfigNode::const_iterator::operator++()
{
    ++d->yamlItr;
    return *this;
}

McConfigNode::const_iterator McConfigNode::const_iterator::operator++(int)
{
    const_iterator tmp(d->yamlItr);
    ++(*this);
    return tmp;
}

bool McConfigNode::const_iterator::operator==(const const_iterator &rhs) const
{
    return d->yamlItr == rhs.d->yamlItr;
}

bool McConfigNode::const_iterator::operator!=(const const_iterator &rhs) const
{
    return d->yamlItr != rhs.d->yamlItr;
}

McConfigNode McConfigNode::const_iterator::operator*() const
{
    McConfigNode node;
    try {
        if (d->yamlItr->IsDefined()) {
            node = McConfigNode(d->yamlItr->as<YAML::Node>());
        } else {
            node[McConfigNode(d->yamlItr->first)] = McConfigNode(d->yamlItr->second);
        }
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
    }
    return node;
}

McConfigNode::iterator McConfigNode::begin() noexcept
{
    return iterator(d->yamlNode.begin());
}

McConfigNode::const_iterator McConfigNode::begin() const noexcept
{
    return const_iterator(d->yamlNode.begin());
}

McConfigNode::const_iterator McConfigNode::constBegin() const noexcept
{
    return const_iterator(d->yamlNode.begin());
}

McConfigNode::const_iterator McConfigNode::cbegin() const noexcept
{
    return const_iterator(d->yamlNode.begin());
}

McConfigNode::iterator McConfigNode::end() noexcept
{
    return iterator(d->yamlNode.end());
}

McConfigNode::const_iterator McConfigNode::end() const noexcept
{
    return const_iterator(d->yamlNode.end());
}

McConfigNode::const_iterator McConfigNode::constEnd() const noexcept
{
    return const_iterator(d->yamlNode.end());
}

McConfigNode::const_iterator McConfigNode::cend() const noexcept
{
    return const_iterator(d->yamlNode.end());
}

void McConfigNode::append(const QVariant &rhs) noexcept
{
    try {
        d->yamlNode.push_back(rhs);
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
    }
    sync_helper();
}

void McConfigNode::append(const McConfigNode &rhs) noexcept
{
    try {
        d->yamlNode.push_back(rhs.d->yamlNode);
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
    }
    initChildNode(rhs);
    sync_helper();
}

const McConfigNode McConfigNode::operator[](const QString &key) const noexcept
{
    try {
        auto node = McConfigNode(d->yamlNode[key]);
        initChildNode(node);
        return node;
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
        return McConfigNode();
    }
}

McConfigNode McConfigNode::operator[](const QString &key) noexcept
{
    try {
        auto node = McConfigNode(d->yamlNode[key]);
        initChildNode(node);
        return node;
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
        return McConfigNode();
    }
}

bool McConfigNode::remove(const QString &key) noexcept
{
    try {
        bool flag = d->yamlNode.remove(key);
        sync_helper();
        return flag;
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
        return false;
    }
}

const McConfigNode McConfigNode::operator[](const McConfigNode &key) const noexcept
{
    try {
        auto node = McConfigNode(d->yamlNode[key.d->yamlNode]);
        initChildNode(node);
        return node;
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
        return McConfigNode();
    }
}

McConfigNode McConfigNode::operator[](const McConfigNode &key) noexcept
{
    try {
        auto node = McConfigNode(d->yamlNode[key.d->yamlNode]);
        initChildNode(node);
        return node;
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
        return McConfigNode();
    }
}

bool McConfigNode::remove(const McConfigNode &key) noexcept
{
    try {
        bool flag = d->yamlNode.remove(key.d->yamlNode);
        sync_helper();
        return flag;
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
        return false;
    }
}

void McConfigNode::insert(const QString &key, const QVariant &value) noexcept
{
    try {
        d->yamlNode.force_insert(key, value);
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
    }
    sync_helper();
}

void McConfigNode::insert(const QString &key, const McConfigNode &value) noexcept
{
    try {
        d->yamlNode.force_insert(key, value.d->yamlNode);
    } catch (const std::exception &e) {
        qCDebug(mcCore, "yaml exception: %s", e.what());
    }
    initChildNode(value);
    sync_helper();
}

void McConfigNode::initChildNode(const McConfigNode &node) const noexcept
{
    node.d->isAutoSync = d->isAutoSync;
    node.d->configPath = d->configPath;
    node.d->syncFunc = d->syncFunc;
}

void McConfigNode::setSyncFunc() noexcept
{
    d->syncFunc = std::bind(&writeYamlNode, d->yamlNode, std::placeholders::_1);
}

void McConfigNode::sync_helper() noexcept
{
    if (!d->isAutoSync)
        return;
    sync();
}

namespace Mc {
McConfigNode &globalConfig(const QString &configPath) noexcept
{
    static McConfigNode node = McConfigNode::loadOrCreate(configPath);
    return node;
}
} // namespace Mc
