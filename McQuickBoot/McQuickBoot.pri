QT *= quick quickwidgets scxml

INCLUDEPATH += $$PWD/include/

SOURCES += \
    $$PWD/src/Application/McSingleApplication.cpp \
    $$PWD/src/Application/McSingleCoreApplication.cpp \
    $$PWD/src/Configuration/McConfigurationContainer.cpp \
    $$PWD/src/Configuration/McLogConfig.cpp \
    $$PWD/src/Configuration/McRequestorConfig.cpp \
    $$PWD/src/Configuration/McStateMachineConfig.cpp \
    $$PWD/src/Configuration/McWidgetConfig.cpp \
    $$PWD/src/Connection/McAbstractConnection.cpp \
    $$PWD/src/Connection/McCppConnection.cpp \
    $$PWD/src/Connection/McQmlConnection.cpp \
    $$PWD/src/Controller/InnerController/McApplicationController.cpp \
    $$PWD/src/Controller/McAbstractResponse.cpp \
    $$PWD/src/Controller/McControllerContainer.cpp \
    $$PWD/src/Controller/McCppResponse.cpp \
    $$PWD/src/Controller/McQmlResponse.cpp \
    $$PWD/src/Controller/McRequestRunner.cpp \
    $$PWD/src/Controller/McResult.cpp \
    $$PWD/src/McAbstractQuickBoot.cpp \
    $$PWD/src/McBootGlobal.cpp \
    $$PWD/src/McQuickBoot.cpp \
    $$PWD/src/McQuickBootSimple.cpp \
    $$PWD/src/Model/McModelContainer.cpp \
    $$PWD/src/Requestor/McAbstractRequestor.cpp \
    $$PWD/src/Requestor/McCppRequestor.cpp \
    $$PWD/src/Requestor/McQmlRequestor.cpp \
    $$PWD/src/Requestor/McRequest.cpp \
    $$PWD/src/Service/McServiceContainer.cpp \
    $$PWD/src/Utils/Callback/McAbstractAsyncCallback.cpp \
    $$PWD/src/Utils/Callback/McAbstractSyncCallback.cpp \
    $$PWD/src/Utils/Callback/McCppAsyncCallback.cpp \
    $$PWD/src/Utils/Callback/McCppSyncCallback.cpp \
    $$PWD/src/Utils/Callback/McQmlSyncCallback.cpp \
    $$PWD/src/Utils/McCancel.cpp \
    $$PWD/src/Utils/McJsonUtils.cpp \
    $$PWD/src/BeanDefinitionReader/McConfigurationFileBeanDefinitionReader.cpp

HEADERS +=  \
    $$PWD/include/McBoot/Application/McSingleApplication.h \
    $$PWD/include/McBoot/Application/McSingleCoreApplication.h \
    $$PWD/include/McBoot/Configuration/McConfigurationContainer.h \
    $$PWD/include/McBoot/Configuration/McLogConfig.h \
    $$PWD/include/McBoot/Configuration/McRequestorConfig.h \
    $$PWD/include/McBoot/Configuration/McSimpleBeanDefinition.h \
    $$PWD/include/McBoot/Configuration/McStateMachineConfig.h \
    $$PWD/include/McBoot/Configuration/McWidgetConfig.h \
    $$PWD/include/McBoot/Connection/McAbstractConnection.h \
    $$PWD/include/McBoot/Connection/McCppConnection.h \
    $$PWD/include/McBoot/Connection/McQmlConnection.h \
    $$PWD/include/McBoot/Controller/IMcControllerContainer.h \
    $$PWD/include/McBoot/Controller/InnerController/McApplicationController.h \
    $$PWD/include/McBoot/Controller/impl/McAbstractResponse.h \
    $$PWD/include/McBoot/Controller/impl/McControllerContainer.h \
    $$PWD/include/McBoot/Controller/impl/McCppResponse.h \
    $$PWD/include/McBoot/Controller/impl/McQmlResponse.h \
    $$PWD/include/McBoot/Controller/impl/McRequestRunner.h \
    $$PWD/include/McBoot/Controller/impl/McResult.h \
    $$PWD/include/McBoot/IMcQuickBoot.h \
    $$PWD/include/McBoot/McAbstractQuickBoot.h \
    $$PWD/include/McBoot/McBootConstantGlobal.h \
    $$PWD/include/McBoot/McBootGlobal.h \
    $$PWD/include/McBoot/McBootMacroGlobal.h \
    $$PWD/include/McBoot/McQuickBoot.h \
    $$PWD/include/McBoot/McQuickBootSimple.h \
    $$PWD/include/McBoot/Model/IMcModelContainer.h \
    $$PWD/include/McBoot/Model/impl/McModelContainer.h \
    $$PWD/include/McBoot/Requestor/McAbstractRequestor.h \
    $$PWD/include/McBoot/Requestor/McCppRequestor.h \
    $$PWD/include/McBoot/Requestor/McQmlRequestor.h \
    $$PWD/include/McBoot/Requestor/McRequest.h \
    $$PWD/include/McBoot/Service/IMcServiceLongLiveThread.h \
    $$PWD/include/McBoot/Service/IMcServiceTimer.h \
    $$PWD/include/McBoot/Service/impl/McServiceContainer.h \
    $$PWD/include/McBoot/Utils/Callback/IMcCallback.h \
    $$PWD/include/McBoot/Utils/Callback/Impl/McAbstractAsyncCallback.h \
    $$PWD/include/McBoot/Utils/Callback/Impl/McAbstractSyncCallback.h \
    $$PWD/include/McBoot/Utils/Callback/Impl/McCppAsyncCallback.h \
    $$PWD/include/McBoot/Utils/Callback/Impl/McCppSyncCallback.h \
    $$PWD/include/McBoot/Utils/Callback/Impl/McQmlSyncCallback.h \
    $$PWD/include/McBoot/Utils/McCancel.h \
    $$PWD/include/McBoot/Utils/McJsonUtils.h \
    $$PWD/include/McBoot/BeanDefinitionReader/impl/McConfigurationFileBeanDefinitionReader.h

RESOURCES += \
    $$PWD/qrc/resource.qrc
