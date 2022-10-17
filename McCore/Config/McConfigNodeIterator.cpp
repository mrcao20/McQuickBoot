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
#include "McConfigNodeIterator.h"

#include <yaml-cpp/yaml.h>

////////////////////////////////////////////
MC_DECL_PRIVATE_DATA2(McConfigNodeIterator)
{
    YAML::iterator yamlItr;
};

McConfigNodeIterator::McConfigNodeIterator() noexcept
{
    MC_NEW_PRIVATE_DATA(McConfigNodeIterator);
}

McConfigNodeIterator::McConfigNodeIterator(YAML::iterator yamlItr) noexcept
    : McConfigNodeIterator()
{
    d->yamlItr = yamlItr;
}

McConfigNodeIterator::~McConfigNodeIterator() {}

McConfigNodeIterator::McConfigNodeIterator(const McConfigNodeIterator &o) noexcept
    : McConfigNodeIterator(o.d->yamlItr)
{
}

McConfigNodeIterator &McConfigNodeIterator::operator=(const McConfigNodeIterator &o) noexcept
{
    d->yamlItr = o.d->yamlItr;
    return *this;
}

McConfigNodeIterator::McConfigNodeIterator(McConfigNodeIterator &&o) noexcept
    : McConfigNodeIterator(o.d->yamlItr)
{
    o.d.reset();
}

McConfigNodeIterator &McConfigNodeIterator::operator=(McConfigNodeIterator &&o) noexcept
{
    d->yamlItr = o.d->yamlItr;

    o.d.reset();
    return *this;
}

McConfigNodeIterator &McConfigNodeIterator::operator++()
{
    ++d->yamlItr;
    return *this;
}

McConfigNodeIterator McConfigNodeIterator::operator++(int)
{
    McConfigNodeIterator tmp(d->yamlItr);
    ++(*this);
    return tmp;
}

bool McConfigNodeIterator::operator==(const McConfigNodeIterator &rhs) const
{
    return d->yamlItr == rhs.d->yamlItr;
}

bool McConfigNodeIterator::operator!=(const McConfigNodeIterator &rhs) const
{
    return d->yamlItr != rhs.d->yamlItr;
}

// McConfigNode McConfigNodeIterator::operator*() const {}

// McConfigNode *McConfigNodeIterator::operator->() const {}

////////////////////////////////////////////////
MC_DECL_PRIVATE_DATA2(McConfigNodeConstIterator)
{
    YAML::const_iterator yamlItr;
};

McConfigNodeConstIterator::McConfigNodeConstIterator() noexcept
{
    MC_NEW_PRIVATE_DATA(McConfigNodeConstIterator);
}

McConfigNodeConstIterator::McConfigNodeConstIterator(YAML::const_iterator yamlItr) noexcept
    : McConfigNodeConstIterator()
{
    d->yamlItr = yamlItr;
}

McConfigNodeConstIterator::~McConfigNodeConstIterator() {}

McConfigNodeConstIterator::McConfigNodeConstIterator(const McConfigNodeConstIterator &o) noexcept
    : McConfigNodeConstIterator(o.d->yamlItr)
{
}

McConfigNodeConstIterator &McConfigNodeConstIterator::operator=(const McConfigNodeConstIterator &o) noexcept
{
    d->yamlItr = o.d->yamlItr;
    return *this;
}

McConfigNodeConstIterator::McConfigNodeConstIterator(McConfigNodeConstIterator &&o) noexcept
    : McConfigNodeConstIterator(o.d->yamlItr)
{
    o.d.reset();
}

McConfigNodeConstIterator &McConfigNodeConstIterator::operator=(McConfigNodeConstIterator &&o) noexcept
{
    d->yamlItr = o.d->yamlItr;

    o.d.reset();
    return *this;
}

McConfigNodeConstIterator &McConfigNodeConstIterator::operator++()
{
    ++d->yamlItr;
    return *this;
}

McConfigNodeConstIterator McConfigNodeConstIterator::operator++(int)
{
    McConfigNodeConstIterator tmp(d->yamlItr);
    ++(*this);
    return tmp;
}

bool McConfigNodeConstIterator::operator==(const McConfigNodeConstIterator &rhs) const
{
    return d->yamlItr == rhs.d->yamlItr;
}

bool McConfigNodeConstIterator::operator!=(const McConfigNodeConstIterator &rhs) const
{
    return d->yamlItr != rhs.d->yamlItr;
}
