INCLUDEPATH += $$PWD/include/

HEADERS += \
    $$PWD/include/Config.h \
    $$PWD/include/Config2.h \
    $$PWD/include/Param.h \
    $$PWD/include/Test.h

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/src/Config.cpp \
    $$PWD/src/Config2.cpp \
    $$PWD/src/Param.cpp \
    $$PWD/src/Test.cpp

RESOURCES += \
    $$PWD/qrc/qml/qml.qrc
	
OTHER_FILES += \
    $$PWD/qrc/config/application.yml
    
InputFilePath = $$PWD/qrc/config/application.yml
InputFilePath = $$replace(InputFilePath, /, \\)
OutputDir = $${DESTDIR}/config
OutputDir = $$replace(OutputDir, /, \\)
!exists($$OutputDir) {
    mkpath($$OutputDir)
}
OutputFilePath = $${DESTDIR}/config/application.yml
OutputFilePath = $$replace(OutputFilePath, /, \\)
QMAKE_POST_LINK += \
    $${QMAKE_COPY_FILE} $$InputFilePath $$OutputFilePath
