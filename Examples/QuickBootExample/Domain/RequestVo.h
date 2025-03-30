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

#include <McQuickBoot/McQuickBootGlobal.h>

struct RequestVo
{
    Q_GADGET
    //! 通过json序列化方式传递数据。通过qml调用C++函数时，只能传递json，如果调用的函数的参数是C++对象，
    //! 则必须使用序列化标志，框架会自动构造C++对象，并使用json数据为其赋值。
    MC_JSON_SERIALIZATION()
public:
    MC_POCO_PROPERTY(QString, req)
};

MC_DECL_METATYPE(RequestVo)
