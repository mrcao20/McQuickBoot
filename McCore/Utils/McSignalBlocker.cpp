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
#include "McSignalBlocker.h"

void McSignalRecursiveBlocker::block_helper(QObject *o, int depth, bool flag) noexcept
{
    if (depth == 0) {
        return;
    }
    o->blockSignals(flag);
    --depth;
    auto children = o->children();
    for (auto &child : qAsConst(children)) {
        block_helper(child, depth, flag);
    }
}