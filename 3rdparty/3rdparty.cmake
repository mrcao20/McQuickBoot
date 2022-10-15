######################### zlib ############################
list(APPEND PROJECT_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/zlib/zlib.h
    ${CMAKE_CURRENT_LIST_DIR}/zlib/crc32.h
    ${CMAKE_CURRENT_LIST_DIR}/zlib/deflate.h
    ${CMAKE_CURRENT_LIST_DIR}/zlib/gzguts.h
    ${CMAKE_CURRENT_LIST_DIR}/zlib/inffast.h
    ${CMAKE_CURRENT_LIST_DIR}/zlib/inffixed.h
    ${CMAKE_CURRENT_LIST_DIR}/zlib/inflate.h
    ${CMAKE_CURRENT_LIST_DIR}/zlib/inftrees.h
    ${CMAKE_CURRENT_LIST_DIR}/zlib/trees.h
    ${CMAKE_CURRENT_LIST_DIR}/zlib/zutil.h
    ${CMAKE_CURRENT_LIST_DIR}/zlib/zip.h
    ${CMAKE_CURRENT_LIST_DIR}/zlib/unzip.h
    ${CMAKE_CURRENT_LIST_DIR}/zlib/ioapi.h
    ${CMAKE_CURRENT_LIST_DIR}/zlib/crypt.h
)

list(APPEND PROJECT_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/zlib/adler32.c
    ${CMAKE_CURRENT_LIST_DIR}/zlib/compress.c
    ${CMAKE_CURRENT_LIST_DIR}/zlib/crc32.c
    ${CMAKE_CURRENT_LIST_DIR}/zlib/deflate.c
    ${CMAKE_CURRENT_LIST_DIR}/zlib/gzclose.c
    ${CMAKE_CURRENT_LIST_DIR}/zlib/gzlib.c
    ${CMAKE_CURRENT_LIST_DIR}/zlib/gzread.c
    ${CMAKE_CURRENT_LIST_DIR}/zlib/gzwrite.c
    ${CMAKE_CURRENT_LIST_DIR}/zlib/inflate.c
    ${CMAKE_CURRENT_LIST_DIR}/zlib/infback.c
    ${CMAKE_CURRENT_LIST_DIR}/zlib/inftrees.c
    ${CMAKE_CURRENT_LIST_DIR}/zlib/inffast.c
    ${CMAKE_CURRENT_LIST_DIR}/zlib/trees.c
    ${CMAKE_CURRENT_LIST_DIR}/zlib/uncompr.c
    ${CMAKE_CURRENT_LIST_DIR}/zlib/zutil.c
    ${CMAKE_CURRENT_LIST_DIR}/zlib/zip.c
    ${CMAKE_CURRENT_LIST_DIR}/zlib/unzip.c
    ${CMAKE_CURRENT_LIST_DIR}/zlib/ioapi.c
)
######################### end zlib ########################

include_directories(${CMAKE_CURRENT_LIST_DIR})
