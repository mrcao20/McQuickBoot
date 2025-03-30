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

#include <QSharedPointer>

#include "../McMacroGlobal.h"

/*!
 * \brief The IMcDestroyer class
 *
 * 对象销毁器，当对象实现至该接口时，会将destroy函数传入QSharedPointer
 * 即，当引用计数为0时，QSharedPointer的析构器会调用destroy函数。
 * 此接口用于自定义删除对象。
 */
class IMcDestroyer
{
    MC_DEFINE_INTERFACE(IMcDestroyer)
public:
    virtual void destroy() = 0;
};

MC_DECL_METATYPE(IMcDestroyer)
