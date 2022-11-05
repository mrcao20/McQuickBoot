INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/google_breakpad/common/minidump_format.h \
    $$PWD/google_breakpad/common/breakpad_types.h \
    $$PWD/google_breakpad/common/minidump_cpu_amd64.h \
    $$PWD/google_breakpad/common/minidump_cpu_arm.h \
    $$PWD/google_breakpad/common/minidump_cpu_arm64.h \
    $$PWD/google_breakpad/common/minidump_cpu_mips.h \
    $$PWD/google_breakpad/common/minidump_cpu_ppc.h \
    $$PWD/google_breakpad/common/minidump_cpu_ppc64.h \
    $$PWD/google_breakpad/common/minidump_cpu_riscv.h \
    $$PWD/google_breakpad/common/minidump_cpu_sparc.h \
    $$PWD/google_breakpad/common/minidump_cpu_x86.h \
    $$PWD/google_breakpad/common/minidump_exception_fuchsia.h \
    $$PWD/google_breakpad/common/minidump_exception_linux.h \
    $$PWD/google_breakpad/common/minidump_exception_mac.h \
    $$PWD/google_breakpad/common/minidump_exception_ps3.h \
    $$PWD/google_breakpad/common/minidump_exception_solaris.h \
    $$PWD/google_breakpad/common/minidump_exception_win32.h \
    $$PWD/common/scoped_ptr.h

win32 {
    HEADERS += \
        $$PWD/common/windows/string_utils-inl.h \
        $$PWD/common/windows/guid_string.h \
        $$PWD/client/windows/handler/exception_handler.h \
        $$PWD/client/windows/common/ipc_protocol.h \
        $$PWD/client/windows/crash_generation/crash_generation_client.h \

    SOURCES += \
        $$PWD/common/windows/guid_string.cc \
        $$PWD/client/windows/handler/exception_handler.cc \
        $$PWD/client/windows/crash_generation/crash_generation_client.cc
} else: macos {
} else: unix {
} else {
    error(platform not support)
}
