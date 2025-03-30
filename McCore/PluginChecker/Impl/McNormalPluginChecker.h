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

#include <QJsonObject>

#include "../IMcPluginChecker.h"

class MC_CORE_EXPORT McNormalPluginChecker : public IMcPluginChecker
{
    MC_FULL_DEFINE(McNormalPluginChecker, IMcPluginChecker)
public:
    McNormalPluginChecker(const QJsonObject &val) noexcept;

    bool check(const QJsonObject &json) noexcept override;

    bool operator()(const QJsonObject &json);

private:
    QJsonObject m_checkJson;
};

MC_DECL_METATYPE(McNormalPluginChecker)
