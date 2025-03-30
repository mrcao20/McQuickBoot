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

#include <McIoc/BeanBuilder/IMcCustomPlaceholder.h>
#include <McIoc/McIocGlobal.h>

#include "../CoreExample/SimpleInterface.h"

class SimplePlugin
    : public QObject
    , public SimpleInterface
    , public IMcCustomPlaceholder
{
    Q_OBJECT
    Q_INTERFACES(SimpleInterface)
    Q_PLUGIN_METADATA(IID SimpleInterfaceIID FILE "../CoreExample/ExamplePluginData.json")
    MC_FULL_DEFINE(SimplePlugin, QObject, SimpleInterface, IMcCustomPlaceholder)
public:
    void simpleFunc() override;
    QVariant getKey() const noexcept override;
};

MC_DECL_METATYPE(SimplePlugin)
