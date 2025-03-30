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

#include <QString>

namespace Mc {
namespace Constant {
namespace Tag {
namespace Xml {
[[maybe_unused]] constexpr const QLatin1String BEANS("beans", 5);
[[maybe_unused]] constexpr const QLatin1String BEAN("bean", 4);
[[maybe_unused]] constexpr const QLatin1String NAME("name", 4);
[[maybe_unused]] constexpr const QLatin1String CLASS("class", 5);
[[maybe_unused]] constexpr const QLatin1String PLUGIN("plugin", 6);
[[maybe_unused]] constexpr const QLatin1String PLUGINS("plugins", 7);
[[maybe_unused]] constexpr const QLatin1String IS_SINGLETON("isSingleton", 11);
[[maybe_unused]] constexpr const QLatin1String IS_POINTER("isPointer", 9);
[[maybe_unused]] constexpr const QLatin1String PARENT("parent", 6);
[[maybe_unused]] constexpr const QLatin1String FLAG("flag", 4);
[[maybe_unused]] constexpr const QLatin1String CLASS_LIST("List", 4);
[[maybe_unused]] constexpr const QLatin1String LIST("list", 4);
[[maybe_unused]] constexpr const QLatin1String CLASS_MAP("Map", 3);
[[maybe_unused]] constexpr const QLatin1String MAP("map", 3);
[[maybe_unused]] constexpr const QLatin1String PLH("plh", 3);
[[maybe_unused]] constexpr const QLatin1String VALUE("value", 5);
[[maybe_unused]] constexpr const QLatin1String REF("ref", 3);
[[maybe_unused]] constexpr const QLatin1String ENUM("enum", 4);
[[maybe_unused]] constexpr const QLatin1String SCOPE("scope", 5);
[[maybe_unused]] constexpr const QLatin1String ENTRY("entry", 5);
[[maybe_unused]] constexpr const QLatin1String KEY("key", 3);
[[maybe_unused]] constexpr const QLatin1String PROPERTY("property", 8);
[[maybe_unused]] constexpr const QLatin1String CONSTRUCTOR_ARG("constructor-arg", 15);
[[maybe_unused]] constexpr const QLatin1String INDEX("index", 5);
[[maybe_unused]] constexpr const QLatin1String METADATA("metaData", 8);
[[maybe_unused]] constexpr const QLatin1String CHECKER("checker", 7);
[[maybe_unused]] constexpr const QLatin1String CONNECT("connect", 7);
[[maybe_unused]] constexpr const QLatin1String THIS("this", 4);
[[maybe_unused]] constexpr const QLatin1String SENDER("sender", 6);
[[maybe_unused]] constexpr const QLatin1String SIGNAL_TAG("signal", 6);
[[maybe_unused]] constexpr const QLatin1String RECEIVER("receiver", 8);
[[maybe_unused]] constexpr const QLatin1String SLOT_TAG("slot", 4);
[[maybe_unused]] constexpr const QLatin1String TYPE("type", 4);
} // namespace Xml
} // namespace Tag
} // namespace Constant
} // namespace Mc
