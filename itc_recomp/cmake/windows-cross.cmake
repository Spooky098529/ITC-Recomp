# Cross-compiling the Windows build on Linux, with clang targeting the MSVC
# ABI and lld-link. The Microsoft CRT and Windows SDK come from
# `xwin splat --use-winsysroot-style --preserve-ms-arch-notation` (see
# docs/windows.md); point XWIN_DIR at its output. clang's
# -Xmicrosoft-windows-sys-root (clang-cl's /winsysroot) then finds the
# headers, after its own built-in ones, and the libraries.

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR AMD64)

if(NOT XWIN_DIR)
    set(XWIN_DIR "${CMAKE_CURRENT_LIST_DIR}/../out/xwin-sdk")
endif()
get_filename_component(XWIN_DIR "${XWIN_DIR}" ABSOLUTE)
# try_compile projects reread this file; pass the directory on to them.
list(APPEND CMAKE_TRY_COMPILE_PLATFORM_VARIABLES XWIN_DIR)
# xwin leaves out the debug CRT (msvcrtd.lib), which Debug test builds link.
set(CMAKE_TRY_COMPILE_CONFIGURATION Release)

set(ITC_WIN_TARGET x86_64-pc-windows-msvc)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER_TARGET ${ITC_WIN_TARGET})
set(CMAKE_CXX_COMPILER_TARGET ${ITC_WIN_TARGET})
set(CMAKE_RC_COMPILER llvm-rc)
set(CMAKE_MT llvm-mt)
# CMake drives GNU-style clang's archiver with ar arguments (qc).
set(CMAKE_AR llvm-ar)
set(CMAKE_RANLIB llvm-ranlib)
set(CMAKE_LINKER lld-link)

# The preset leaves CMAKE_<LANG>_FLAGS unset so these reach the compiler.
set(ITC_WIN_FLAGS "-march=x86-64-v2 -Xmicrosoft-windows-sys-root \"${XWIN_DIR}\"")
set(CMAKE_C_FLAGS_INIT "${ITC_WIN_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${ITC_WIN_FLAGS}")

file(GLOB ITC_WIN_SDK_INCLUDE LIST_DIRECTORIES true "${XWIN_DIR}/Windows Kits/10/Include/*")
file(GLOB ITC_WIN_CRT_INCLUDE LIST_DIRECTORIES true "${XWIN_DIR}/VC/Tools/MSVC/*/include")
# Resources (SDL's version resource) need the SDK headers too. CMake
# preprocesses them with clang++ before llvm-rc, so -I, not /I.
set(CMAKE_RC_FLAGS_INIT "")
foreach(dir "${ITC_WIN_SDK_INCLUDE}/um" "${ITC_WIN_SDK_INCLUDE}/shared"
        "${ITC_WIN_SDK_INCLUDE}/ucrt" "${ITC_WIN_CRT_INCLUDE}")
    string(APPEND CMAKE_RC_FLAGS_INIT " -I \"${dir}\"")
endforeach()

set(ITC_WIN_LINK_FLAGS "-fuse-ld=lld -Xmicrosoft-windows-sys-root \"${XWIN_DIR}\"")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${ITC_WIN_LINK_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "${ITC_WIN_LINK_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "${ITC_WIN_LINK_FLAGS}")

set(CMAKE_FIND_ROOT_PATH "${XWIN_DIR}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
