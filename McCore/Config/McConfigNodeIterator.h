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
namespace detail {
struct iterator_value;
template<typename V>
class iterator_base;
} // namespace detail

using iterator = detail::iterator_base<detail::iterator_value>;
using const_iterator = detail::iterator_base<const detail::iterator_value>;
} // namespace YAML

class McConfigNode;

//////////////////////////////////////////////
MC_FORWARD_DECL_PRIVATE_DATA(McConfigNodeIterator)

class MC_CORE_EXPORT McConfigNodeIterator
{
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;

    McConfigNodeIterator() noexcept;
    McConfigNodeIterator(YAML::iterator yamlItr) noexcept;
    ~McConfigNodeIterator();

    McConfigNodeIterator(const McConfigNodeIterator &o) noexcept;
    McConfigNodeIterator &operator=(const McConfigNodeIterator &o) noexcept;
    McConfigNodeIterator(McConfigNodeIterator &&o) noexcept;
    McConfigNodeIterator &operator=(McConfigNodeIterator &&o) noexcept;

    McConfigNodeIterator &operator++();
    McConfigNodeIterator operator++(int);

    bool operator==(const McConfigNodeIterator &rhs) const;
    bool operator!=(const McConfigNodeIterator &rhs) const;

    McConfigNode operator*() const;
    McConfigNode *operator->() const;

private:
    MC_DECL_PRIVATE(McConfigNodeIterator)
};

///////////////////////////////////////////////////
MC_FORWARD_DECL_PRIVATE_DATA(McConfigNodeConstIterator)

class MC_CORE_EXPORT McConfigNodeConstIterator
{
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;

    McConfigNodeConstIterator() noexcept;
    McConfigNodeConstIterator(YAML::const_iterator yamlItr) noexcept;
    ~McConfigNodeConstIterator();

    McConfigNodeConstIterator(const McConfigNodeConstIterator &o) noexcept;
    McConfigNodeConstIterator &operator=(const McConfigNodeConstIterator &o) noexcept;
    McConfigNodeConstIterator(McConfigNodeConstIterator &&o) noexcept;
    McConfigNodeConstIterator &operator=(McConfigNodeConstIterator &&o) noexcept;

    McConfigNodeConstIterator &operator++();
    McConfigNodeConstIterator operator++(int);

    bool operator==(const McConfigNodeConstIterator &rhs) const;
    bool operator!=(const McConfigNodeConstIterator &rhs) const;

    QVariant operator*() const;
    QVariant *operator->() const;

private:
    MC_DECL_PRIVATE(McConfigNodeConstIterator)
};
