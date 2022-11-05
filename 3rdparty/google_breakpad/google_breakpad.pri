INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/third_party/lss/linux_syscall_support.h \
    $$PWD/google_breakpad/common/minidump_size.h \
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
    $$PWD/common/scoped_ptr.h \
    $$PWD/common/basictypes.h \
    $$PWD/common/using_std_string.h \
    $$PWD/common/memory_range.h \
    $$PWD/common/memory_allocator.h \
    $$PWD/common/minidump_type_helper.h \
    $$PWD/common/macros.h \
    $$PWD/common/convert_UTF.h \
    $$PWD/common/string_conversion.h

SOURCES += \
    $$PWD/common/convert_UTF.cc \
    $$PWD/common/string_conversion.cc

win32 {
    HEADERS += \
        $$PWD/common/windows/string_utils-inl.h \
        $$PWD/common/windows/guid_string.h \
        $$PWD/client/windows/handler/exception_handler.h \
        $$PWD/client/windows/common/ipc_protocol.h \
        $$PWD/client/windows/crash_generation/crash_generation_client.h

    SOURCES += \
        $$PWD/common/windows/guid_string.cc \
        $$PWD/client/windows/handler/exception_handler.cc \
        $$PWD/client/windows/crash_generation/crash_generation_client.cc
} else: macos {
} else: unix {
    HEADERS += \
        $$PWD/common/linux/breakpad_getcontext.h \
        $$PWD/common/linux/ucontext_constants.h \
        $$PWD/common/linux/eintr_wrapper.h \
        $$PWD/common/linux/ignore_ret.h \
        $$PWD/common/linux/file_id.h \
        $$PWD/common/linux/guid_creator.h \
        $$PWD/common/linux/linux_libc_support.h \
        $$PWD/common/linux/elf_gnu_compat.h \
        $$PWD/common/linux/elfutils.h \
        $$PWD/common/linux/elfutils-inl.h \
        $$PWD/common/linux/memory_mapped_file.h \
        $$PWD/common/linux/safe_readlink.h \
        $$PWD/client/minidump_file_writer.h \
        $$PWD/client/minidump_file_writer-inl.h \
        $$PWD/client/linux/handler/exception_handler.h \
        $$PWD/client/linux/handler/minidump_descriptor.h \
        $$PWD/client/linux/handler/microdump_extra_info.h \
        $$PWD/client/linux/minidump_writer/minidump_writer.h \
        $$PWD/client/linux/minidump_writer/linux_dumper.h \
        $$PWD/client/linux/minidump_writer/linux_ptrace_dumper.h \
        $$PWD/client/linux/minidump_writer/directory_reader.h \
        $$PWD/client/linux/minidump_writer/line_reader.h \
        $$PWD/client/linux/minidump_writer/cpu_set.h \
        $$PWD/client/linux/minidump_writer/pe_file.h \
        $$PWD/client/linux/minidump_writer/pe_structs.h \
        $$PWD/client/linux/minidump_writer/proc_cpuinfo_reader.h \
        $$PWD/client/linux/dump_writer_common/mapping_info.h \
        $$PWD/client/linux/dump_writer_common/thread_info.h \
        $$PWD/client/linux/dump_writer_common/raw_context_cpu.h \
        $$PWD/client/linux/dump_writer_common/ucontext_reader.h \
        $$PWD/client/linux/crash_generation/crash_generation_client.h \
        $$PWD/client/linux/log/log.h \
        $$PWD/client/linux/microdump_writer/microdump_writer.h

    SOURCES += \
        $$PWD/common/linux/breakpad_getcontext.S \
        $$PWD/common/linux/file_id.cc \
        $$PWD/common/linux/guid_creator.cc \
        $$PWD/common/linux/linux_libc_support.cc \
        $$PWD/common/linux/elfutils.cc \
        $$PWD/common/linux/memory_mapped_file.cc \
        $$PWD/common/linux/safe_readlink.cc \
        $$PWD/client/minidump_file_writer.cc \
        $$PWD/client/linux/handler/exception_handler.cc \
        $$PWD/client/linux/handler/minidump_descriptor.cc \
        $$PWD/client/linux/minidump_writer/minidump_writer.cc \
        $$PWD/client/linux/minidump_writer/linux_dumper.cc \
        $$PWD/client/linux/minidump_writer/linux_ptrace_dumper.cc \
        $$PWD/client/linux/minidump_writer/pe_file.cc \
        $$PWD/client/linux/dump_writer_common/thread_info.cc \
        $$PWD/client/linux/dump_writer_common/ucontext_reader.cc \
        $$PWD/client/linux/crash_generation/crash_generation_client.cc \
        $$PWD/client/linux/log/log.cc \
        $$PWD/client/linux/microdump_writer/microdump_writer.cc
} else {
    error(platform not support)
}
