INCLUDEPATH += $$PWD

DEFINES += MC_USE_LIBRARY # 如果将此库放在程序的某一个库中，其他模块链接到该库使用时打开此配置
#DEFINES += MC_EXPORT_DISABLE # 如果将此库直接链接到程序，不放在任何库中时打开此配置
#DEFINES += MC_DISABLE_ZLIB # 此库内置了zlib源码，如果不希望如此，则开启此选项

include(3rdparty/3rdparty.pri)
include(McCore/McCore.pri)
include(McIoc/McIoc.pri)
include(McLog/McLog.pri)
include(McQuickBoot/McQuickBoot.pri)
include(McWidget/McWidget.pri)
