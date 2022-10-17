INCLUDEPATH += $$PWD

DEFINES += MC_USE_LIBRARY # 如果将此库放在程序的某一个库中，其他模块链接到该库使用时打开此配置
#DEFINES += MC_EXPORT_DISABLE # 如果将此库直接链接到程序，不放在任何库中时打开此配置

include(3rdparty/3rdparty.pri)
include(McCore/McCore.pri)
include(McIoc/McIoc.pri)
include(McLog/McLog.pri)
include(McQuickBoot/McQuickBoot.pri)
include(McWidget/McWidget.pri)
