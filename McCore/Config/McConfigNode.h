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
#pragma once

#include "../McGlobal.h"
//#include "McConfigNodeIterator.h"

namespace YAML {
class Node;
}

MC_FORWARD_DECL_PRIVATE_DATA(McConfigNode)

class MC_CORE_EXPORT McConfigNode
{
public:
    enum class NodeType {
        Undefined,
        Null,
        Scalar,
        Sequence,
        Map,
        Pair, //!< 当YAML Node类型为Map，但是只有一个键值对时即为此类型
    };

    McConfigNode() noexcept;
    explicit McConfigNode(NodeType type) noexcept;
    McConfigNode(const YAML::Node &yamlNode) noexcept;
    template<typename T>
    explicit McConfigNode(const T &rhs) noexcept
        : McConfigNode(McPrivate::toQVariant(rhs))
    {
    }
    explicit McConfigNode(const QVariant &rhs) noexcept;
    ~McConfigNode();

    McConfigNode(const McConfigNode &o) noexcept;
    McConfigNode &operator=(const McConfigNode &o) noexcept;
    McConfigNode(McConfigNode &&o) noexcept;
    McConfigNode &operator=(McConfigNode &&o) noexcept;

    static McConfigNode loadOrCreate(const QString &configPath) noexcept;

    void sync() noexcept;

    NodeType type() const noexcept;
    bool isDefined() const noexcept;
    bool isNull() const noexcept { return type() == NodeType::Null; }
    bool isScalar() const noexcept { return type() == NodeType::Scalar; }
    bool isSequence() const noexcept { return type() == NodeType::Sequence; }
    bool isMap() const noexcept { return type() == NodeType::Map; }
    //! 由于yaml没有pair类型，所以这个函数是判断节点是map类型，但size等于1的情况
    bool isPair() const noexcept { return type() == NodeType::Pair; }

    //! bool conversions
    bool operator!() const noexcept { return !isDefined(); }

    //! access
    //! 如果需要读取的类型为map，尽量使用asMap函数，因为当只有一个键值对时内部将其处理为了pair而导致无法转换成map
    template<typename T>
    T as() const noexcept
    {
        QVariant var = as();
        return var.value<T>();
    }
    QVariant as() const noexcept;
    template<typename T, typename S>
    T as(const S &fallback) const noexcept
    {
        QVariant var = as(McPrivate::toQVariant(fallback));
        return var.value<T>();
    }
    QVariant as(const QVariant &fallback) const noexcept;

    template<typename T>
    T asMap() const noexcept
    {
        QVariant var = asMap();
        return var.value<T>();
    }
    QVariant asMap() const noexcept;
    template<typename T, typename S>
    T asMap(const S &fallback) const noexcept
    {
        QVariant var = asMap(McPrivate::toQVariant(fallback));
        return var.value<T>();
    }
    QVariant asMap(const QVariant &fallback) const noexcept;

    template<typename T>
    T asPair() const noexcept
    {
        QVariant var = asPair();
        return var.value<T>();
    }
    QVariant asPair() const noexcept;
    template<typename T, typename S>
    T asPair(const S &fallback) const noexcept
    {
        QVariant var = asPair(McPrivate::toQVariant(fallback));
        return var.value<T>();
    }
    QVariant asPair(const QVariant &fallback) const noexcept;

    QString scalar() const noexcept;
    template<typename T>
    operator T() const noexcept
    {
        return as<T>();
    }

    //! assignment
    bool is(const McConfigNode &rhs) const noexcept;
    template<typename T>
    McConfigNode &operator=(const T &rhs) noexcept
    {
        return operator=(McPrivate::toQVariant(rhs));
    }
    McConfigNode &operator=(const QVariant &rhs) noexcept;
    void reset(const McConfigNode &rhs = McConfigNode()) noexcept;

    //! size/iterator
    std::size_t size() const noexcept;
    //! 注意此函数取出的节点只能用于读取数据，写入数据无效
    McConfigNode at(std::size_t index) const noexcept;

    //! 注意类型为map或pair时才能获取所有键
    QStringList keys() const noexcept;
    //! 注意类型为pair才能使用key/value函数
    QString key() const noexcept;
    McConfigNode value() const noexcept;

    //! 迭代器暂时无法实现
    //    using iterator = McConfigNodeIterator;
    //    using const_iterator = McConfigNodeConstIterator;

    //    iterator begin() noexcept;
    //    const_iterator begin() const noexcept;
    //    const_iterator constBegin() const noexcept;
    //    const_iterator cbegin() const noexcept;
    //    iterator end() noexcept;
    //    const_iterator end() const noexcept;
    //    const_iterator constEnd() const noexcept;
    //    const_iterator cend() const noexcept;

    //! sequence
    template<typename T>
    void append(const T &rhs) noexcept
    {
        append(McPrivate::toQVariant(rhs));
    }
    void append(const QVariant &rhs) noexcept;
    void append(const McConfigNode &rhs) noexcept;

    //! indexing
    template<typename Key>
    const McConfigNode operator[](const Key &key) const noexcept
    {
        return operator[](QString(key));
    }
    template<typename Key>
    McConfigNode operator[](const Key &key) noexcept
    {
        return operator[](QString(key));
    }
    template<typename Key>
    bool remove(const Key &key) noexcept
    {
        return remove(QString(key));
    }

    const McConfigNode operator[](const QString &key) const noexcept;
    McConfigNode operator[](const QString &key) noexcept;
    bool remove(const QString &key) noexcept;

    const McConfigNode operator[](const McConfigNode &key) const noexcept;
    McConfigNode operator[](const McConfigNode &key) noexcept;
    bool remove(const McConfigNode &key) noexcept;

    //! map
    template<typename Value>
    void insert(const QString &key, const Value &value) noexcept
    {
        insert(key, McPrivate::toQVariant(value));
    }
    void insert(const QString &key, const QVariant &value) noexcept;
    void insert(const QString &key, const McConfigNode &value) noexcept;

private:
    void setRootNode(const McConfigNode &node) const noexcept;

private:
    MC_DECL_PRIVATE(McConfigNode)
};

namespace Mc {
/*!
 * \brief globalConfig
 * \param configPath 配置文件路径，只需要在第一次调用时传递，用于初始化配置文件，之后传递此参数不起作用。
 * 如果第一次调用不传递则使用默认配置路径(./config/runtimeConfig.yml)
 * \return
 */
MC_CORE_EXPORT McConfigNode &globalConfig(const QString &configPath = QString()) noexcept;
} // namespace Mc
