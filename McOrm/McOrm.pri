QT *= sql

INCLUDEPATH += $$PWD/include/

HEADERS += $$PWD/include/McOrm/McDll.h \
    $$PWD/include/McOrm/DaoLoader/IMcDaoLoader.h \
    $$PWD/include/McOrm/DaoLoader/impl/McDaoLoader.h \
    $$PWD/include/McOrm/McOrmMacroGlobal.h \
    $$PWD/include/McOrm/McOrmGlobal.h \
    $$PWD/include/McOrm/Parser/IMcParser.h \
    $$PWD/include/McOrm/Parser/impl/McAbstractParser.h \
    $$PWD/include/McOrm/Parser/impl/McSelectParser.h \
    $$PWD/include/McOrm/SqlSlot/IMcSqlSlot.h \
    $$PWD/include/McOrm/SqlSlot/impl/McAbstractSqlSlot.h \
    $$PWD/include/McOrm/SqlSlot/impl/McSelectSqlSlot.h

SOURCES += \
    $$PWD/src/DaoLoader/McDaoLoader.cpp \
    $$PWD/src/McOrmGlobal.cpp \
    $$PWD/src/Parser/McAbstractParser.cpp \
    $$PWD/src/Parser/McSelectParser.cpp \
    $$PWD/src/SqlSlot/McAbstractSqlSlot.cpp \
    $$PWD/src/SqlSlot/McSelectSqlSlot.cpp
