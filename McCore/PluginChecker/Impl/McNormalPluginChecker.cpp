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
#include "McNormalPluginChecker.h"

MC_AUTO_INIT(McNormalPluginChecker)
MC_INIT_END

McNormalPluginChecker::McNormalPluginChecker(const QJsonObject &val) noexcept
    : m_checkJson(val)
{
}

bool McNormalPluginChecker::check(const QJsonObject &json) noexcept
{
    auto checkKeys = m_checkJson.keys();
    for (auto &checkKey : qAsConst(checkKeys)) {
        if (!json.contains(checkKey)) {
            return false;
        }
        auto checkValue = m_checkJson.value(checkKey);
        auto value = json.value(checkKey);
        if (checkValue != value) {
            return false;
        }
    }
    return true;
}

bool McNormalPluginChecker::operator()(const QJsonObject &json)
{
    return check(json);
}
