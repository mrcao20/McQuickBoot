SOURCES += \\
    $$PWD/%{MainCppFileName}

RESOURCES += \\
    $$PWD/qrc/qml/qml.qrc
	
OTHER_FILES += \\
    $$PWD/qrc/config/application.yml
    
InputFilePath = $$PWD/qrc/config/application.yml
InputFilePath = $$replace(InputFilePath, /, \\\\)
OutputDir = $${DESTDIR}/config
OutputDir = $$replace(OutputDir, /, \\\\)
CONFIG(debug, debug|release) {
    OutputFilePath = $${DESTDIR}/application.yml
    OutputFilePath = $$replace(OutputFilePath, /, \\\\)
    QMAKE_POST_LINK += \\
        $${QMAKE_COPY_FILE} $$InputFilePath $$OutputFilePath
} else {
    OutputFilePath = $${DESTDIR}/config/application.yml
    OutputFilePath = $$replace(OutputFilePath, /, \\\\)
    QMAKE_POST_LINK += \\
        $${QMAKE_MKDIR} $$OutputDir && \\
        $${QMAKE_COPY_FILE} $$InputFilePath $$OutputFilePath
}
