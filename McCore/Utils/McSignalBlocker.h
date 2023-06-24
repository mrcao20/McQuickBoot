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

/*!
 * \brief 本文件中的两个工具类和QSignalBlocker有所不同，QSignalBlocker在析构时可以恢复到对象之前的信号锁定状态。
 * 即如果之前对象未被锁定，QSignalBlocker析构后对象也未被锁定；如果之前对象被锁定，QSignalBlocker析构后对象仍然被锁定。
 *
 * 而本文件中的两个工具类在析构之后，目标对象将全部解锁，不管对象之前是否被锁定。
 */

#include <QObject>

#include "../McMacroGlobal.h"

/////////////////////////////////////
class McSignalBlockers
{
public:
    inline McSignalBlockers() noexcept;
    inline explicit McSignalBlockers(const QList<QObject *> &os) noexcept;
    inline ~McSignalBlockers();

    inline void addObject(QObject *o) noexcept;

    inline void reblock() noexcept;
    inline void unblock() noexcept;

private:
    Q_DISABLE_COPY_MOVE(McSignalBlockers)

    QList<QObject *> m_os;
};

McSignalBlockers::McSignalBlockers() noexcept {}

McSignalBlockers::McSignalBlockers(const QList<QObject *> &os) noexcept
    : m_os(os)
{
    reblock();
}

McSignalBlockers::~McSignalBlockers()
{
    unblock();
}

inline void McSignalBlockers::addObject(QObject *o) noexcept
{
    m_os.append(o);
    o->blockSignals(true);
}

inline void McSignalBlockers::reblock() noexcept
{
    for (auto &o : m_os) {
        o->blockSignals(true);
    }
}

inline void McSignalBlockers::unblock() noexcept
{
    for (auto &o : m_os) {
        o->blockSignals(false);
    }
}

/////////////////////////////////////
class MC_CORE_EXPORT McSignalRecursiveBlocker
{
public:
    /*!
     * \brief McSignalRecursiveBlocker
     * \param o
     * \param depth 0表示不锁定；正数表示递归层级，当前对象算第一级；负数表示一直递归到最后一层
     */
    inline explicit McSignalRecursiveBlocker(QObject *o, int depth = 1) noexcept;
    inline ~McSignalRecursiveBlocker();

    inline void reblock() noexcept;
    inline void unblock() noexcept;

private:
    void block_helper(QObject *o, int depth, bool flag) noexcept;

private:
    Q_DISABLE_COPY_MOVE(McSignalRecursiveBlocker)

    QObject *m_o;
    int m_depth;
};

McSignalRecursiveBlocker::McSignalRecursiveBlocker(QObject *o, int depth) noexcept
    : m_o(o)
    , m_depth(depth)
{
    reblock();
}

McSignalRecursiveBlocker::~McSignalRecursiveBlocker()
{
    unblock();
}

inline void McSignalRecursiveBlocker::reblock() noexcept
{
    block_helper(m_o, m_depth, true);
}

inline void McSignalRecursiveBlocker::unblock() noexcept
{
    block_helper(m_o, m_depth, false);
}
