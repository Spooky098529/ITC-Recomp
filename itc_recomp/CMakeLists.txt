# itc - ReXGlue Recompiled Project
#
# SDK boilerplate lives in generated/rexglue.cmake.

cmake_minimum_required(VERSION 3.25)
project(itc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Apply this project's patches to the rexglue-sdk submodule, skipping any
# already applied. See patches/rexglue-sdk/ for what each one works around.
if(REXSDK_DIR)
    file(GLOB ITC_SDK_PATCHES "${CMAKE_CURRENT_SOURCE_DIR}/../patches/rexglue-sdk/*.patch")
    list(SORT ITC_SDK_PATCHES)
    # Patches touching nearby lines can't each be checked alone once a later
    # one is applied. They're always applied in order, so if the last one is
    # applied, they all are.
    list(GET ITC_SDK_PATCHES -1 last_patch)
    execute_process(
        COMMAND git -C "${REXSDK_DIR}" apply --reverse --check "${last_patch}"
        RESULT_VARIABLE all_applied OUTPUT_QUIET ERROR_QUIET)
    if(all_applied EQUAL 0)
        set(ITC_SDK_PATCHES "")
    endif()
    foreach(patch IN LISTS ITC_SDK_PATCHES)
        execute_process(
            COMMAND git -C "${REXSDK_DIR}" apply --reverse --check "${patch}"
            RESULT_VARIABLE already_applied OUTPUT_QUIET ERROR_QUIET)
        if(NOT already_applied EQUAL 0)
            execute_process(
                COMMAND git -C "${REXSDK_DIR}" apply "${patch}"
                RESULT_VARIABLE apply_result)
            if(NOT apply_result EQUAL 0)
                message(FATAL_ERROR "Failed to apply SDK patch ${patch}")
            endif()
            message(STATUS "Applied SDK patch ${patch}")
        endif()
    endforeach()
endif()

# ThinLTO with lld, for the game and the SDK (set before the SDK is added).
option(ITC_LTO "Build with ThinLTO (needs lld)" OFF)
if(ITC_LTO)
    add_compile_options(-flto=thin)
    add_link_options(-flto=thin -fuse-ld=lld
        -Wl,--thinlto-cache-dir=${CMAKE_BINARY_DIR}/thinlto-cache)
endif()

# ThinLTO for the Windows build, kept apart from ITC_LTO: lld-link takes
# its own options (/lldltocache, not --thinlto-cache-dir).
option(ITC_WIN_LTO "Windows: build with ThinLTO (lld-link)" OFF)
if(WIN32 AND ITC_WIN_LTO)
    add_compile_options(-flto=thin)
    add_link_options(-flto=thin "LINKER:/lldltocache:${CMAKE_BINARY_DIR}/thinlto-cache")
endif()

include(generated/rexglue.cmake)

# Sources
set(ITC_SOURCES
    src/main.cpp
    src/audio/native_audio.cpp
    src/hooks/vehicle.cpp
    src/hooks/title_screen.cpp
    src/hooks/create_file.cpp
    src/hooks/lights.cpp
    src/hooks/cockpit_dials.cpp
    src/hooks/input.cpp
    src/hooks/input_update.cpp
    src/hooks/debug_output.cpp
    src/hooks/object_attributes.cpp
    src/hooks/time.cpp
    src/helpers/debug_output.cpp
    src/helpers/object_attributes.cpp
    src/helpers/time.cpp
    src/input/gamepads.cpp
    src/input/input_processor.cpp
    src/input/logical_pad.cpp
    src/input/physical_pad.cpp
    src/input/xinput.cpp
    src/frame_pacing/frame_pacer.cpp
    src/frame_pacing/renderer_hooks.cpp
    src/debug/state_capture.cpp
    src/debug/frame_state_diff.cpp
    src/debug/idle_timer_scan.cpp
    src/debug/menu_selection_scan.cpp
    src/debug/pause_menu_log.cpp
    src/debug/pause_movie_log.cpp
    src/debug/image_dump.cpp
    src/debug/replay_stats.cpp
    src/debug/thread_cpu.cpp
    src/native/app_window.cpp
    src/native/buffers.cpp
    src/native/disk_cache.cpp
    src/native/draw_packets.cpp
    src/native/gpu_link.cpp
    src/native/passes.cpp
    src/native/refcount.cpp
    src/native/renderer.cpp
    src/native/replay_hooks.cpp
    src/native/shaders.cpp
    src/native/state.cpp
    src/native/textures.cpp
    src/native/vulkan_functions.cpp
    src/interpolation/transform.cpp
    src/frame_pacing/game_messages.cpp
    src/frame_pacing/per_frame_logic.cpp
    src/interpolation/blend_state.cpp
    src/interpolation/glow.cpp
)

if(WIN32)
    add_executable(itc WIN32 ${ITC_SOURCES})
else()
    add_executable(itc ${ITC_SOURCES})
endif()

rexglue_setup_target(itc GPU_PLUGINS xenos)

target_include_directories(itc PRIVATE src)

# The SDK's Xenos shader translator (to SPIR-V), built into our binary for
# the native renderer (docs/native-renderer.md: reused for now, to be
# replaced). The plugin has its own copy with hidden symbols.
set(ITC_SDK_GRAPHICS ${REXSDK_DIR}/src/graphics)
add_library(itc_xenos_shaders STATIC
    ${ITC_SDK_GRAPHICS}/pipeline/shader/shader.cpp
    ${ITC_SDK_GRAPHICS}/pipeline/shader/translator.cpp
    ${ITC_SDK_GRAPHICS}/pipeline/shader/translator_disasm.cpp
    ${ITC_SDK_GRAPHICS}/pipeline/shader/spirv_builder.cpp
    ${ITC_SDK_GRAPHICS}/pipeline/shader/spirv.cpp
    ${ITC_SDK_GRAPHICS}/pipeline/shader/spirv_translator.cpp
    ${ITC_SDK_GRAPHICS}/pipeline/shader/spirv_translator_alu.cpp
    ${ITC_SDK_GRAPHICS}/pipeline/shader/spirv_translator_fetch.cpp
    ${ITC_SDK_GRAPHICS}/pipeline/shader/spirv_translator_memexport.cpp
    ${ITC_SDK_GRAPHICS}/pipeline/shader/spirv_translator_rb.cpp
    ${ITC_SDK_GRAPHICS}/format/ucode.cpp
    ${ITC_SDK_GRAPHICS}/util/draw.cpp
    ${ITC_SDK_GRAPHICS}/registers.cpp
    ${ITC_SDK_GRAPHICS}/register_file.cpp
)
target_include_directories(itc_xenos_shaders PUBLIC ${REXSDK_DIR}/thirdparty/renderdoc)
set_target_properties(itc_xenos_shaders PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON)
target_compile_options(itc_xenos_shaders PRIVATE -Wno-unused-variable)
target_link_libraries(itc_xenos_shaders PUBLIC
    rexruntime xxHash::xxhash Vulkan::Headers glslang::SPIRV glslang
    MachineIndependent GenericCodeGen OSDependent OGLCompiler)
target_link_libraries(itc PRIVATE itc_xenos_shaders)

# The native renderer (src/native/) uses SDL, which lives in librexruntime,
# and loads Vulkan at runtime: it needs only the headers. Both are the SDK's
# third-party copies.
target_include_directories(itc PRIVATE
    ${REXSDK_DIR}/thirdparty/sdl3/include
    ${REXSDK_DIR}/thirdparty/vulkan-headers/include)

# rex::runtime doesn't propagate imgui's include path, which rex_app.cpp and
# the app header need. Headers only: the symbols already live in librexruntime.
target_include_directories(itc PRIVATE
    $<TARGET_PROPERTY:imgui::imgui,INTERFACE_INCLUDE_DIRECTORIES>)
