include_directories(${CMAKE_CURRENT_LIST_DIR})

list(APPEND PROJECT_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/google_breakpad/common/minidump_format.h
    ${CMAKE_CURRENT_LIST_DIR}/google_breakpad/common/breakpad_types.h
    ${CMAKE_CURRENT_LIST_DIR}/google_breakpad/common/minidump_cpu_amd64.h
    ${CMAKE_CURRENT_LIST_DIR}/google_breakpad/common/minidump_cpu_arm.h
    ${CMAKE_CURRENT_LIST_DIR}/google_breakpad/common/minidump_cpu_arm64.h
    ${CMAKE_CURRENT_LIST_DIR}/google_breakpad/common/minidump_cpu_mips.h
    ${CMAKE_CURRENT_LIST_DIR}/google_breakpad/common/minidump_cpu_ppc.h
    ${CMAKE_CURRENT_LIST_DIR}/google_breakpad/common/minidump_cpu_ppc64.h
    ${CMAKE_CURRENT_LIST_DIR}/google_breakpad/common/minidump_cpu_riscv.h
    ${CMAKE_CURRENT_LIST_DIR}/google_breakpad/common/minidump_cpu_sparc.h
    ${CMAKE_CURRENT_LIST_DIR}/google_breakpad/common/minidump_cpu_x86.h
    ${CMAKE_CURRENT_LIST_DIR}/google_breakpad/common/minidump_exception_fuchsia.h
    ${CMAKE_CURRENT_LIST_DIR}/google_breakpad/common/minidump_exception_linux.h
    ${CMAKE_CURRENT_LIST_DIR}/google_breakpad/common/minidump_exception_mac.h
    ${CMAKE_CURRENT_LIST_DIR}/google_breakpad/common/minidump_exception_ps3.h
    ${CMAKE_CURRENT_LIST_DIR}/google_breakpad/common/minidump_exception_solaris.h
    ${CMAKE_CURRENT_LIST_DIR}/google_breakpad/common/minidump_exception_win32.h

    ${CMAKE_CURRENT_LIST_DIR}/common/scoped_ptr.h
)

if(WIN32)
    list(APPEND PROJECT_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/common/windows/string_utils-inl.h
        ${CMAKE_CURRENT_LIST_DIR}/common/windows/guid_string.h
        ${CMAKE_CURRENT_LIST_DIR}/client/windows/handler/exception_handler.h
        ${CMAKE_CURRENT_LIST_DIR}/client/windows/common/ipc_protocol.h
        ${CMAKE_CURRENT_LIST_DIR}/client/windows/crash_generation/crash_generation_client.h
    )

    list(APPEND PROJECT_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/common/windows/guid_string.cc
        ${CMAKE_CURRENT_LIST_DIR}/client/windows/handler/exception_handler.cc
        ${CMAKE_CURRENT_LIST_DIR}/client/windows/crash_generation/crash_generation_client.cc
    )
elseif(UNIX)
else()
    message(FATAL_ERROR "platform not support")
endif()
