INCLUDEPATH += $$PWD/include/

SOURCES += \
    src/Application/McSingleApplication.cpp \
    src/Application/McSingleCoreApplication.cpp \
    src/Controller/InnerController/McApplicationController.cpp \
    src/Controller/McControllerContainer.cpp \
    src/Controller/McQmlResponse.cpp \
    src/Controller/McRequestRunner.cpp \
    src/Controller/McResult.cpp \
    src/McIocBoot.cpp \
    src/Model/McModelContainer.cpp \
    src/Requestor/McQmlRequestor.cpp \
    src/Socket/McInnerSocket.cpp \
    src/Socket/McQmlSocket.cpp \
    src/Socket/McQmlSocketContainer.cpp \
    src/Socket/McQmlSocketRunner.cpp \
    src/Socket/McSession.cpp \
    src/Utils/McJsonUtils.cpp

HEADERS +=  \
    include/McBoot/Application/McSingleApplication.h \
    include/McBoot/Application/McSingleCoreApplication.h \
    include/McBoot/Controller/IMcControllerContainer.h \
    include/McBoot/Controller/InnerController/McApplicationController.h \
    include/McBoot/Controller/impl/McControllerContainer.h \
    include/McBoot/Controller/impl/McQmlResponse.h \
    include/McBoot/Controller/impl/McRequestRunner.h \
    include/McBoot/Controller/impl/McResult.h \
    include/McBoot/McBootGlobal.h \
    include/McBoot/McBootMacroGlobal.h \
    include/McBoot/McBootVersion.h \
    include/McBoot/McIocBoot.h \
    include/McBoot/Model/McModelContainer.h \
    include/McBoot/Requestor/McQmlRequestor.h \
    include/McBoot/Socket/IMcQmlSocketContainer.h \
    include/McBoot/Socket/IMcSession.h \
    include/McBoot/Socket/impl/McInnerSocket.h \
    include/McBoot/Socket/impl/McQmlSocket.h \
    include/McBoot/Socket/impl/McQmlSocketContainer.h \
    include/McBoot/Socket/impl/McQmlSocketRunner.h \
    include/McBoot/Socket/impl/McSession.h \
    include/McBoot/Utils/McJsonUtils.h
