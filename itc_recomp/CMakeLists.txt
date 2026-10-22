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
include(cmake/ffmpeg.cmake)

# Sources
set(ITC_SOURCES
    src/main.cpp
    src/base/log.cpp
    src/base/log_sdk.cpp
    src/audio/native_audio.cpp
    src/audio/output_mix.cpp
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
    src/hooks/clock.cpp
    src/hooks/xma.cpp
    src/hooks/audio_volume.cpp
    src/hooks/strings.cpp
    src/hooks/last_error.cpp
    src/hooks/gamma.cpp
    src/hooks/graphics_drops.cpp
    src/hooks/graphics_device.cpp
    src/hooks/video_mode.cpp
    src/hooks/start_up.cpp
    src/hooks/user.cpp
    src/hooks/xmp.cpp
    src/hooks/network.cpp
    src/hooks/notify.cpp
    src/hooks/profile.cpp
    src/hooks/saves.cpp
    src/saves/saves.cpp
    src/hooks/file_class.cpp
    src/hooks/movie_files.cpp
    src/hooks/archive_streaming.cpp
    src/hooks/memory_status.cpp
    src/hooks/voice.cpp
    src/hooks/sprites.cpp
    src/sprites/sprites.cpp
    src/hooks/billboards.cpp
    src/sprites/billboards.cpp
    src/hooks/ui_movies.cpp
    src/ui_movies/ui_movies.cpp
    src/ui_movies/keys.cpp
    src/ui_movies/recording.cpp
    src/ui_movies/files.cpp
    src/hooks/ui_movies_game.cpp
    src/ui_movies/game.cpp
    src/hooks/race_overlays.cpp
    src/race_overlays/race_overlays.cpp
    src/hooks/vehicle_accessors.cpp
    src/vehicle_accessors/vehicle_accessors.cpp
    src/vehicle_accessors/vectors.cpp
    src/hooks/matrix_math.cpp
    src/matrix_math/matrix_math.cpp
    src/hooks/car_physics.cpp
    src/car_physics/rigid_body.cpp
    src/car_physics/vehicle_step.cpp
    src/hooks/command_list.cpp
    src/command_list/command_list.cpp
    src/hooks/c_runtime.cpp
    src/hooks/math.cpp
    src/c_runtime/math.cpp
    src/c_runtime/strings.cpp
    src/c_runtime/format.cpp
    src/c_runtime/start_up.cpp
    src/c_runtime/parse.cpp
    src/game_data/game_data.cpp
    src/display/gamma.cpp
    src/harnesses/harness.cpp
    src/harnesses/clock.cpp
    src/harnesses/last_error.cpp
    src/harnesses/gamma.cpp
    src/harnesses/object_attributes.cpp
    src/harnesses/strings.cpp
    src/harnesses/time.cpp
    src/harnesses/start_up.cpp
    src/harnesses/user.cpp
    src/harnesses/xmp.cpp
    src/harnesses/network.cpp
    src/harnesses/notify.cpp
    src/harnesses/profile.cpp
    src/harnesses/saves.cpp
    src/harnesses/file_class.cpp
    src/harnesses/movie_files.cpp
    src/harnesses/xma.cpp
    src/harnesses/sprites.cpp
    src/harnesses/billboards.cpp
    src/harnesses/command_list.cpp
    src/harnesses/c_runtime.cpp
    src/harnesses/math.cpp
    src/harnesses/ui_movies.cpp
    src/harnesses/ui_movies_game.cpp
    src/harnesses/race_overlays.cpp
    src/harnesses/vehicle_accessors.cpp
    src/harnesses/matrix_math.cpp
    src/harnesses/car_physics.cpp
    src/helpers/debug_output.cpp
    src/helpers/object_attributes.cpp
    src/helpers/time.cpp
    src/helpers/strings.cpp
    src/helpers/executable.cpp
    src/user/profile.cpp
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

# The C runtime's math (src/c_runtime/math.cpp) must give the translated
# code's doubles bit for bit, NaNs' signs included: std::fma only where the
# PowerPC code fuses (no other contraction), and no rewrites that move a
# negation into an addition (LLVM's default model treats a NaN's sign as
# free; seen with -O3 on asin of a negative NaN). See
# docs/functions/c-runtime.md. The UI movie keys' interpolation
# (src/ui_movies/keys.cpp) likewise (docs/functions/ui-movies.md), and the
# matrix math (src/matrix_math/matrix_math.cpp, docs/functions/matrix-math.md)
# and the car physics (src/car_physics/, docs/functions/car-physics.md).
set_source_files_properties(src/c_runtime/math.cpp src/ui_movies/keys.cpp
    src/matrix_math/matrix_math.cpp src/car_physics/rigid_body.cpp
    src/car_physics/vehicle_step.cpp PROPERTIES
    COMPILE_OPTIONS "-ffp-model=strict;-ffp-contract=off"
    SKIP_PRECOMPILE_HEADERS ON)

# Our XMA decoding (src/audio/xma_*: B2 in docs/rexglue-removal.md) on our
# own FFmpeg, in a library of its own so only our FFmpeg's headers reach it.
add_library(itc_xma STATIC src/audio/xma_packets.cpp src/audio/xma_stream.cpp)
target_include_directories(itc_xma PUBLIC src)
target_link_libraries(itc_xma PRIVATE itc_avcodec)

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

# The game's executable, from our sources and the recompiled code.
function(itc_add_executable name)
    if(WIN32)
        add_executable(${name} WIN32 ${ARGN})
    else()
        add_executable(${name} ${ARGN})
    endif()

    rexglue_setup_target(${name})

    target_include_directories(${name} PRIVATE src)

    # Our own copies of header-only libraries (A1 in docs/rexglue-removal.md),
    # included with their folder (<xxHash/xxhash.h>) so the SDK's copies on the
    # include path can't stand in for them.
    target_include_directories(${name} PRIVATE thirdparty)

    target_link_libraries(${name} PRIVATE itc_xma itc_xenos_shaders)

    # The native renderer (src/native/) uses SDL, which lives in librexruntime,
    # and loads Vulkan at runtime: it needs only the headers. Both are the SDK's
    # third-party copies.
    target_include_directories(${name} PRIVATE
        ${REXSDK_DIR}/thirdparty/sdl3/include
        ${REXSDK_DIR}/thirdparty/vulkan-headers/include)

    # rex::runtime doesn't propagate imgui's include path, which rex_app.cpp and
    # the app header need. Headers only: the symbols already live in librexruntime.
    target_include_directories(${name} PRIVATE
        $<TARGET_PROPERTY:imgui::imgui,INTERFACE_INCLUDE_DIRECTORIES>)
endfunction()

itc_add_executable(itc ${ITC_SOURCES})

# The coverage build: which recompiled functions run in play ("Functions
# seen in play" in docs/rexglue-removal.md). A second executable beside itc,
# not built by default ('cmake --build ... --target itc_coverage'), whose
# copy of the recompiled code marks each function the first time it runs;
# src/debug/coverage.cpp writes the marks out. Its own target rather than a
# build directory of its own, as the SDK writes its libraries to one place
# (rexglue-sdk/out/<platform>) whatever the build directory. Linux only.
option(ITC_COVERAGE "Add the itc_coverage target" OFF)
if(ITC_COVERAGE)
    itc_add_executable(itc_coverage ${ITC_SOURCES} src/debug/coverage.cpp)
    set_target_properties(itc_coverage itc_coverage_recomp PROPERTIES EXCLUDE_FROM_ALL ON)
    # Without LTO: ThinLTO's import leaves the coverage tables' private
    # globals (__sancov_gen_*) undefined at the link.
    target_compile_options(itc_coverage_recomp PRIVATE
        -fsanitize-coverage=func,inline-bool-flag,pc-table -fno-lto)
endif()

# Decodes one XMA wave from a game bank with our FFmpeg (B2 in
# docs/rexglue-removal.md; how to run it is at the top of the file). Not
# built by default.
add_executable(itc_xma_check EXCLUDE_FROM_ALL tests/xma_check.cpp)
target_link_libraries(itc_xma_check PRIVATE itc_xma itc_avcodec)
