/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * ***** END LICENSE BLOCK ***** */

/*
 * Copyright (c) 2014, Filip Brcic <brcha@gna.org>. All rights reserved.
 *
 * This file is part of lusim
 */

#pragma once

#include <yaml-cpp/yaml.h>

#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QSettings>

#include "McDll.h"

class MCYAML_EXPORT McYaml
{
public:
    static void registerYaml() noexcept;
    
    static inline QSettings::Format format() noexcept
    { return m_format; }
    
private:
    static QSettings::Format m_format;
};

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
            return Node(rhs.toMap());
        } else if(rhs.canConvert<QVariantList>()) {
            return Node(rhs.toList());
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
            rhs = node.as<QVariantMap>();
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

// QVector
template<typename T>
struct convert<QVector<T> >
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

// QPair
template<typename T, typename U>
struct convert<QPair<T, U> >
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

// TODO: Add the rest of the container classes
// QLinkedList, QStack, QQueue, QSet, QMultiMap, QHash, QMultiHash, QStringList, ...

} // end namespace YAML
