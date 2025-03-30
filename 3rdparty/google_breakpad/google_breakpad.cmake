include_directories(${CMAKE_CURRENT_LIST_DIR})

list(APPEND PROJECT_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/third_party/lss/linux_syscall_support.h
    ${CMAKE_CURRENT_LIST_DIR}/google_breakpad/common/minidump_size.h
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
    ${CMAKE_CURRENT_LIST_DIR}/common/basictypes.h
    ${CMAKE_CURRENT_LIST_DIR}/common/using_std_string.h
    ${CMAKE_CURRENT_LIST_DIR}/common/memory_range.h
    ${CMAKE_CURRENT_LIST_DIR}/common/memory_allocator.h
    ${CMAKE_CURRENT_LIST_DIR}/common/minidump_type_helper.h
    ${CMAKE_CURRENT_LIST_DIR}/common/macros.h
    ${CMAKE_CURRENT_LIST_DIR}/common/convert_UTF.h
    ${CMAKE_CURRENT_LIST_DIR}/common/string_conversion.h
)

list(APPEND PROJECT_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/common/convert_UTF.cc
    ${CMAKE_CURRENT_LIST_DIR}/common/string_conversion.cc
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
    list(APPEND PROJECT_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/common/linux/breakpad_getcontext.h
        ${CMAKE_CURRENT_LIST_DIR}/common/linux/ucontext_constants.h
        ${CMAKE_CURRENT_LIST_DIR}/common/linux/eintr_wrapper.h
        ${CMAKE_CURRENT_LIST_DIR}/common/linux/ignore_ret.h
        ${CMAKE_CURRENT_LIST_DIR}/common/linux/file_id.h
        ${CMAKE_CURRENT_LIST_DIR}/common/linux/guid_creator.h
        ${CMAKE_CURRENT_LIST_DIR}/common/linux/linux_libc_support.h
        ${CMAKE_CURRENT_LIST_DIR}/common/linux/elf_gnu_compat.h
        ${CMAKE_CURRENT_LIST_DIR}/common/linux/elfutils.h
        ${CMAKE_CURRENT_LIST_DIR}/common/linux/elfutils-inl.h
        ${CMAKE_CURRENT_LIST_DIR}/common/linux/memory_mapped_file.h
        ${CMAKE_CURRENT_LIST_DIR}/common/linux/safe_readlink.h
        ${CMAKE_CURRENT_LIST_DIR}/client/minidump_file_writer.h
        ${CMAKE_CURRENT_LIST_DIR}/client/minidump_file_writer-inl.h
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/handler/exception_handler.h
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/handler/minidump_descriptor.h
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/handler/microdump_extra_info.h
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/minidump_writer/minidump_writer.h
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/minidump_writer/linux_dumper.h
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/minidump_writer/linux_ptrace_dumper.h
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/minidump_writer/directory_reader.h
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/minidump_writer/line_reader.h
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/minidump_writer/cpu_set.h
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/minidump_writer/pe_file.h
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/minidump_writer/pe_structs.h
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/minidump_writer/proc_cpuinfo_reader.h
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/dump_writer_common/mapping_info.h
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/dump_writer_common/thread_info.h
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/dump_writer_common/raw_context_cpu.h
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/dump_writer_common/ucontext_reader.h
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/crash_generation/crash_generation_client.h
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/log/log.h
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/microdump_writer/microdump_writer.h
    )

    list(APPEND PROJECT_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/common/linux/breakpad_getcontext.S
        ${CMAKE_CURRENT_LIST_DIR}/common/linux/file_id.cc
        ${CMAKE_CURRENT_LIST_DIR}/common/linux/guid_creator.cc
        ${CMAKE_CURRENT_LIST_DIR}/common/linux/linux_libc_support.cc
        ${CMAKE_CURRENT_LIST_DIR}/common/linux/elfutils.cc
        ${CMAKE_CURRENT_LIST_DIR}/common/linux/memory_mapped_file.cc
        ${CMAKE_CURRENT_LIST_DIR}/common/linux/safe_readlink.cc
        ${CMAKE_CURRENT_LIST_DIR}/client/minidump_file_writer.cc
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/handler/exception_handler.cc
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/handler/minidump_descriptor.cc
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/minidump_writer/minidump_writer.cc
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/minidump_writer/linux_dumper.cc
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/minidump_writer/linux_ptrace_dumper.cc
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/minidump_writer/pe_file.cc
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/dump_writer_common/thread_info.cc
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/dump_writer_common/ucontext_reader.cc
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/crash_generation/crash_generation_client.cc
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/log/log.cc
        ${CMAKE_CURRENT_LIST_DIR}/client/linux/microdump_writer/microdump_writer.cc
    )
else()
    message(FATAL_ERROR "platform not support")
endif()
