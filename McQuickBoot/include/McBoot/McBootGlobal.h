#pragma once

#include <QLoggingCategory>

#include <McIoc/McGlobal.h>

#include "McBootConstantGlobal.h"
#include "McBootMacroGlobal.h"

#include "McAbstractQuickBoot.h"

//! 此宏所对应的对象将在Application析构时销毁，所以一旦Application开始析构，就再也不要调用此宏
#define $ (McAbstractQuickBoot::instance()->requestor())

#define MC_QUICKBOOT_SERVICE_EXPORT(FUNC) \
    extern "C" Q_DECL_EXPORT void FUNC() {}

namespace Mc {

// 是否添加默认查找路径，默认会到mcservices目录下查找，如果不需要则需要手动调用此函数置为false
MCQUICKBOOT_EXPORT void setDefaultSearch(bool val);
MCQUICKBOOT_EXPORT void setLibraryCheckSymbol(const QLatin1String &symbol);
//! 必须传入全路径，可以是相对与可执行程序的相对路径
MCQUICKBOOT_EXPORT void addServiceSearchPath(const QString &path);
MCQUICKBOOT_EXPORT void addServiceSearchPath(const QStringList &paths);
//! 必须传入动态库全路径，可以是相对与可执行程序的相对路径
MCQUICKBOOT_EXPORT void addServiceLibraryPath(const QString &path);
MCQUICKBOOT_EXPORT void addServiceLibraryPath(const QStringList &paths);

} // namespace Mc

Q_DECLARE_LOGGING_CATEGORY(mcQuickBoot)
