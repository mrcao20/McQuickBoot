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

constexpr const char *beans = "beans";
constexpr const char *bean = "bean";
constexpr const char *property = "property";
constexpr const char *connect = "connect";
constexpr const char *self = "this";
constexpr const char *sender = "sender";
constexpr const char *signal = "signal";
constexpr const char *receiver = "receiver";
constexpr const char *slot = "slot";
constexpr const char *type = "type";

}

namespace QSetting {

constexpr const char *clazz = "Class";
constexpr const char *plugin = "Plugin";
constexpr const char *singleton = "Singleton";
constexpr const char *pointer = "Pointer";
constexpr const char *connects = "Connects";
constexpr const char *sender = "Sender";
constexpr const char *signal = "Signal";
constexpr const char *receiver = "Receiver";
constexpr const char *slot = "Slot";
constexpr const char *type = "Type";
constexpr const char *ref = "$";

}

}

}
}
