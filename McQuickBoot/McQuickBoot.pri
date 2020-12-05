INCLUDEPATH += $$PWD/include/

SOURCES += \
    $$PWD/src/Application/McSingleApplication.cpp \
    $$PWD/src/Application/McSingleCoreApplication.cpp \
    $$PWD/src/Controller/InnerController/McApplicationController.cpp \
    $$PWD/src/Controller/McControllerContainer.cpp \
    $$PWD/src/Controller/McQmlResponse.cpp \
    $$PWD/src/Controller/McRequestRunner.cpp \
    $$PWD/src/Controller/McResult.cpp \
    $$PWD/src/McQuickBoot.cpp \
    $$PWD/src/McQuickBootSimple.cpp \
    $$PWD/src/Model/McModelContainer.cpp \
    $$PWD/src/Requestor/McQmlRequestor.cpp \
    $$PWD/src/Socket/McInnerSocket.cpp \
    $$PWD/src/Socket/McQmlSocket.cpp \
    $$PWD/src/Socket/McQmlSocketContainer.cpp \
    $$PWD/src/Socket/McQmlSocketRunner.cpp \
    $$PWD/src/Socket/McSession.cpp \
    $$PWD/src/Utils/McJsonUtils.cpp \
    $$PWD/src/BeanDefinitionReader/McConfigurationFileBeanDefinitionReader.cpp \
    $$PWD/src/Configuration/McQmlRequestorConfig.cpp

HEADERS +=  \
    $$PWD/include/McBoot/Application/McSingleApplication.h \
    $$PWD/include/McBoot/Application/McSingleCoreApplication.h \
    $$PWD/include/McBoot/Controller/IMcControllerContainer.h \
    $$PWD/include/McBoot/Controller/InnerController/McApplicationController.h \
    $$PWD/include/McBoot/Controller/impl/McControllerContainer.h \
    $$PWD/include/McBoot/Controller/impl/McQmlResponse.h \
    $$PWD/include/McBoot/Controller/impl/McRequestRunner.h \
    $$PWD/include/McBoot/Controller/impl/McResult.h \
    $$PWD/include/McBoot/IMcQuickBoot.h \
    $$PWD/include/McBoot/McBootGlobal.h \
    $$PWD/include/McBoot/McBootMacroGlobal.h \
    $$PWD/include/McBoot/McQuickBoot.h \
    $$PWD/include/McBoot/McQuickBootSimple.h \
    $$PWD/include/McBoot/Model/McModelContainer.h \
    $$PWD/include/McBoot/Requestor/McQmlRequestor.h \
    $$PWD/include/McBoot/Socket/IMcQmlSocketContainer.h \
    $$PWD/include/McBoot/Socket/IMcSession.h \
    $$PWD/include/McBoot/Socket/impl/McInnerSocket.h \
    $$PWD/include/McBoot/Socket/impl/McQmlSocket.h \
    $$PWD/include/McBoot/Socket/impl/McQmlSocketContainer.h \
    $$PWD/include/McBoot/Socket/impl/McQmlSocketRunner.h \
    $$PWD/include/McBoot/Socket/impl/McSession.h \
    $$PWD/include/McBoot/Utils/McJsonUtils.h \
    $$PWD/include/McBoot/BeanDefinitionReader/impl/McConfigurationFileBeanDefinitionReader.h \
    $$PWD/include/McBoot/Configuration/McQmlRequestorConfig.h
