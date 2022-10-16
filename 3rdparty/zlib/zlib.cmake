list(APPEND PROJECT_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/zlib.h
    ${CMAKE_CURRENT_LIST_DIR}/crc32.h
    ${CMAKE_CURRENT_LIST_DIR}/deflate.h
    ${CMAKE_CURRENT_LIST_DIR}/gzguts.h
    ${CMAKE_CURRENT_LIST_DIR}/inffast.h
    ${CMAKE_CURRENT_LIST_DIR}/inffixed.h
    ${CMAKE_CURRENT_LIST_DIR}/inflate.h
    ${CMAKE_CURRENT_LIST_DIR}/inftrees.h
    ${CMAKE_CURRENT_LIST_DIR}/trees.h
    ${CMAKE_CURRENT_LIST_DIR}/zutil.h
    ${CMAKE_CURRENT_LIST_DIR}/zip.h
    ${CMAKE_CURRENT_LIST_DIR}/unzip.h
    ${CMAKE_CURRENT_LIST_DIR}/ioapi.h
    ${CMAKE_CURRENT_LIST_DIR}/crypt.h
)

list(APPEND PROJECT_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/adler32.c
    ${CMAKE_CURRENT_LIST_DIR}/compress.c
    ${CMAKE_CURRENT_LIST_DIR}/crc32.c
    ${CMAKE_CURRENT_LIST_DIR}/deflate.c
    ${CMAKE_CURRENT_LIST_DIR}/gzclose.c
    ${CMAKE_CURRENT_LIST_DIR}/gzlib.c
    ${CMAKE_CURRENT_LIST_DIR}/gzread.c
    ${CMAKE_CURRENT_LIST_DIR}/gzwrite.c
    ${CMAKE_CURRENT_LIST_DIR}/inflate.c
    ${CMAKE_CURRENT_LIST_DIR}/infback.c
    ${CMAKE_CURRENT_LIST_DIR}/inftrees.c
    ${CMAKE_CURRENT_LIST_DIR}/inffast.c
    ${CMAKE_CURRENT_LIST_DIR}/trees.c
    ${CMAKE_CURRENT_LIST_DIR}/uncompr.c
    ${CMAKE_CURRENT_LIST_DIR}/zutil.c
    ${CMAKE_CURRENT_LIST_DIR}/zip.c
    ${CMAKE_CURRENT_LIST_DIR}/unzip.c
    ${CMAKE_CURRENT_LIST_DIR}/ioapi.c
)
