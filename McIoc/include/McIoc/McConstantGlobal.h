#pragma once

namespace Mc {
namespace Constant {

namespace Property {

//! Inner QObject Property
//constexpr const char *customDeleter = "__mc__customDeleter";
//!< Inner QObject Property
}

namespace Tag {

namespace Xml {

[[maybe_unused]] constexpr const char *beans = "beans";
[[maybe_unused]] constexpr const char *flag = "flag";
[[maybe_unused]] constexpr const char *bean = "bean";
[[maybe_unused]] constexpr const char *property = "property";
[[maybe_unused]] constexpr const char *connect = "connect";
[[maybe_unused]] constexpr const char *self = "this";
[[maybe_unused]] constexpr const char *sender = "sender";
[[maybe_unused]] constexpr const char *signal = "signal";
[[maybe_unused]] constexpr const char *receiver = "receiver";
[[maybe_unused]] constexpr const char *slot = "slot";
[[maybe_unused]] constexpr const char *type = "type";
}

namespace QSetting {

[[maybe_unused]] constexpr const char *clazz = "Class";
[[maybe_unused]] constexpr const char *plugin = "Plugin";
[[maybe_unused]] constexpr const char *singleton = "Singleton";
[[maybe_unused]] constexpr const char *pointer = "Pointer";
[[maybe_unused]] constexpr const char *connects = "Connects";
[[maybe_unused]] constexpr const char *sender = "Sender";
[[maybe_unused]] constexpr const char *signal = "Signal";
[[maybe_unused]] constexpr const char *receiver = "Receiver";
[[maybe_unused]] constexpr const char *slot = "Slot";
[[maybe_unused]] constexpr const char *type = "Type";
[[maybe_unused]] constexpr const char *ref = "$";
}

}

}
}
