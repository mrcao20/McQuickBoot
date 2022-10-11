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
#include "SimpleObject.h"

#include <QDebug>

MC_AUTO_INIT(SimpleObject)
MC_INIT_END

void SimpleObject::simpleFunc()
{
    qDebug() << "SimpleObject::simpleFunc";
}

void SimpleObject::buildStarted() noexcept
{
    qDebug() << "SimpleObject::buildStarted";
}

void SimpleObject::buildFinished() noexcept
{
    qDebug() << "SimpleObject::buildFinished";
}

void SimpleObject::buildThreadMoved() noexcept
{
    qDebug() << "SimpleObject::buildThreadMoved";
}

void SimpleObject::buildCompleted() noexcept
{
    qDebug() << "SimpleObject::buildCompleted";
}

QVariant SimpleObject::getKey() const noexcept
{
    return "SimpleObjectKey";
}
