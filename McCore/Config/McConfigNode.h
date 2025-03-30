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

namespace YAML {
class Node;

namespace detail {
struct iterator_value;
template<typename V>
class iterator_base;
} // namespace detail

using iterator = detail::iterator_base<detail::iterator_value>;
using const_iterator = detail::iterator_base<const detail::iterator_value>;
}

MC_FORWARD_DECL_PRIVATE_DATA(McConfigNode)
MC_FORWARD_DECL_PRIVATE_DATA(McConfigNodeIterator)
MC_FORWARD_DECL_PRIVATE_DATA(McConfigNodeConstIterator)

/*!
 * \brief The McConfigNode class
 * 此类内部使用了yaml-cpp第三方库，而这个库有一个特性是它的节点(node)和节点所占用的内存(memory)是单独存储的，
 * 并且memory并不会主动释放。这就导致对node的内容不断进行修改，哪怕修改的key是同一个，它的node数量是正常的，
 * 但是memory会一直增大，直到整个Node全部释放才会释放。
 * 所以在使用此类时需要注意：1.直接使用赋值运算符(=)赋值时(哪怕是修改而不是增加)，Node的内存也不会减小；2.如果
 * 你想对节点置空或者真正意义上的赋值(即将要操作的两个节点在赋值之后完全相等)时应该调用reset方法；3.如果你想采
 * 用第2点所说的方法，那需要注意两个将要操作的节点都必须是最上级节点，否则会出现意料之外的结果；4.如果你只是想拷
 * 贝Node中的内容，但又需要保证memory不会出异常，那你应该调用copyContentTo方法，通过此方法操作的两个节点在完
 * 成之后完全独立(节点内容和内存均独立且不相互影响)。
 */
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

    static McConfigNode loadOrCreate(
        const QString &configPath, QIODevice::OpenMode flags = QIODevice::ReadWrite) noexcept;

    bool isAutoSync() const noexcept;
    void setAutoSync(bool val) noexcept;
    void sync() noexcept;
    //! 同步整颗节点树上的所有数据，从根开始
    void syncTo(const QString &path) const noexcept;
    //! 从当前节点开始，同步当前节点及其子节点数据
    void syncCurrentTo(const QString &path) const noexcept;
    /*!
     * \brief configPath
     * 由于存在父子关系悖论，所以不提供setConfigPath这样的方法。
     * 如果想要构造一个包含配置路径的节点，只能调用loadOrCreate方法来实现。
     * 如果想将已有节点的数据保存到某一配置文件路径，可以直接调用已有节点的syncTo或syncCurrentTo方法，或者
     * 调用已有节点的copyContentTo方法将内容拷贝到由loadOrCreate方法创建的新节点中。
     * \return
     */
    QString configPath() const noexcept;
    void copyContentTo(McConfigNode &other) const noexcept;

    NodeType type() const noexcept;
    bool isValid() const noexcept { return isDefined() && !isNull(); }
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
        return McPrivate::toRealValue<T>(var);
    }
    QVariant as() const noexcept;
    template<typename T, typename S>
    T as(const S &fallback) const noexcept
    {
        QVariant var = as(McPrivate::toQVariant(fallback));
        return McPrivate::toRealValue<T>(var);
    }
    QVariant as(const QVariant &fallback) const noexcept;

    template<typename T>
    T asMap() const noexcept
    {
        QVariant var = asMap();
        return McPrivate::toRealValue<T>(var);
    }
    QVariant asMap() const noexcept;
    template<typename T, typename S>
    T asMap(const S &fallback) const noexcept
    {
        QVariant var = asMap(McPrivate::toQVariant(fallback));
        return McPrivate::toRealValue<T>(var);
    }
    QVariant asMap(const QVariant &fallback) const noexcept;

    template<typename T>
    T asPair() const noexcept
    {
        QVariant var = asPair();
        return McPrivate::toRealValue<T>(var);
    }
    QVariant asPair() const noexcept;
    template<typename T, typename S>
    T asPair(const S &fallback) const noexcept
    {
        QVariant var = asPair(McPrivate::toQVariant(fallback));
        return McPrivate::toRealValue<T>(var);
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

    class MC_CORE_EXPORT iterator
    {
        //        struct Proxy
        //        {
        //            explicit Proxy(const McConfigNode &x)
        //                : m_ref(x)
        //            {
        //            }
        //            McConfigNode *operator->() { return std::addressof(m_ref); }
        //            operator McConfigNode *() { return std::addressof(m_ref); }

        //            McConfigNode m_ref;
        //        };

    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;

        iterator() noexcept;
        iterator(const YAML::iterator &yamlItr) noexcept;
        ~iterator();

        iterator(const iterator &o) noexcept;
        iterator &operator=(const iterator &o) noexcept;
        iterator(iterator &&o) noexcept;
        iterator &operator=(iterator &&o) noexcept;

        iterator &operator++();
        iterator operator++(int);

        bool operator==(const iterator &rhs) const;
        bool operator!=(const iterator &rhs) const;

        McConfigNode operator*() const;
        //        Proxy operator->() const;

    private:
        MC_DECL_PRIVATE(McConfigNodeIterator)
    };

    class MC_CORE_EXPORT const_iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;

        const_iterator() noexcept;
        const_iterator(const YAML::const_iterator &yamlItr) noexcept;
        ~const_iterator();

        const_iterator(const const_iterator &o) noexcept;
        const_iterator &operator=(const const_iterator &o) noexcept;
        const_iterator(const_iterator &&o) noexcept;
        const_iterator &operator=(const_iterator &&o) noexcept;

        const_iterator &operator++();
        const_iterator operator++(int);

        bool operator==(const const_iterator &rhs) const;
        bool operator!=(const const_iterator &rhs) const;

        McConfigNode operator*() const;

    private:
        MC_DECL_PRIVATE(McConfigNodeConstIterator)
    };

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator constBegin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator constEnd() const noexcept;
    const_iterator cend() const noexcept;

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
    void initChildNode(const McConfigNode &node) const noexcept;
    void setSyncFunc() noexcept;
    void sync_helper() noexcept;

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
