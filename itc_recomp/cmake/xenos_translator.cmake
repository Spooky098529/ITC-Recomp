# The ReXGlue SDK's Xenos shader translator (to SPIR-V), our one static
# dependency on the SDK (E4's step 5 in docs/rexglue-removal.md; the user's
# call, 2026-11-16): its sources built from the rexglue-sdk submodule into a
# static library, with the few SDK sources holding the helpers it calls. Its
# logging and flags go through our shims (src/translator_shims/), so nothing
# else of the SDK (its runtime, spdlog, its flag registry) is built.

set(REXSDK_DIR "${CMAKE_CURRENT_LIST_DIR}/../../rexglue-sdk" CACHE PATH
    "The rexglue-sdk submodule (the shader translator's source)")
if(NOT EXISTS ${REXSDK_DIR}/src/graphics/pipeline/shader/spirv_translator.cpp)
    message(FATAL_ERROR "Submodule missing: git submodule update --init rexglue-sdk")
endif()

set(ITC_SDK_SRC ${REXSDK_DIR}/src)
add_library(itc_xenos_shaders STATIC
    # The translator.
    ${ITC_SDK_SRC}/graphics/pipeline/shader/shader.cpp
    ${ITC_SDK_SRC}/graphics/pipeline/shader/translator.cpp
    ${ITC_SDK_SRC}/graphics/pipeline/shader/translator_disasm.cpp
    ${ITC_SDK_SRC}/graphics/pipeline/shader/spirv_builder.cpp
    ${ITC_SDK_SRC}/graphics/pipeline/shader/spirv.cpp
    ${ITC_SDK_SRC}/graphics/pipeline/shader/spirv_translator.cpp
    ${ITC_SDK_SRC}/graphics/pipeline/shader/spirv_translator_alu.cpp
    ${ITC_SDK_SRC}/graphics/pipeline/shader/spirv_translator_fetch.cpp
    ${ITC_SDK_SRC}/graphics/pipeline/shader/spirv_translator_memexport.cpp
    ${ITC_SDK_SRC}/graphics/pipeline/shader/spirv_translator_rb.cpp
    ${ITC_SDK_SRC}/graphics/format/ucode.cpp
    ${ITC_SDK_SRC}/graphics/util/draw.cpp
    ${ITC_SDK_SRC}/graphics/registers.cpp
    ${ITC_SDK_SRC}/graphics/register_file.cpp
    # The helpers it calls.
    ${ITC_SDK_SRC}/graphics/xenos.cpp
    ${ITC_SDK_SRC}/graphics/pipeline/texture/info_formats.cpp
    ${ITC_SDK_SRC}/graphics/pipeline/texture/util.cpp
    ${ITC_SDK_SRC}/ui/graphics_util.cpp
    ${ITC_SDK_SRC}/core/string_buffer.cpp
    ${ITC_SDK_SRC}/core/memory.cpp
    ${ITC_SDK_SRC}/core/math_gcc.cpp
    ${ITC_SDK_SRC}/core/filesystem_posix.cpp
    ${REXSDK_DIR}/thirdparty/dxbc/DXBCChecksum.cpp
    # Our shims' definitions.
    ${CMAKE_CURRENT_LIST_DIR}/../src/translator_shims/shims.cpp
)
# Our shims first, so they stand in for the SDK's logging and flag headers.
target_include_directories(itc_xenos_shaders BEFORE PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../src/translator_shims)
target_include_directories(itc_xenos_shaders PUBLIC
    ${REXSDK_DIR}/include
    ${REXSDK_DIR}
    ${REXSDK_DIR}/thirdparty/renderdoc
    ${CMAKE_CURRENT_LIST_DIR}/../src
    ${CMAKE_CURRENT_LIST_DIR}/../thirdparty)
target_compile_definitions(itc_xenos_shaders PUBLIC REX_HAS_VULKAN=1)
set_target_properties(itc_xenos_shaders PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON)
# The flags the SDK built it with (its global options).
target_compile_options(itc_xenos_shaders PRIVATE -Wno-unused-variable
    -fno-strict-aliasing -ffp-model=strict -fno-char8_t)
if(UNIX AND NOT APPLE AND CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    target_compile_options(itc_xenos_shaders PRIVATE -mcmodel=large)
endif()
target_link_libraries(itc_xenos_shaders PUBLIC
    itc_vulkan_headers itc_simde fmt::fmt glslang::SPIRV glslang)
