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
    src/hooks/input_manager.cpp
    src/std_list/ptr_list.cpp
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
    src/hooks/heap.cpp
    src/memory/heap.cpp
    src/hooks/c_runtime_heap.cpp
    src/memory/c_runtime_heap.cpp
    src/hooks/voice.cpp
    src/hooks/sprites.cpp
    src/sprites/sprites.cpp
    src/hooks/billboards.cpp
    src/sprites/billboards.cpp
    src/hooks/ui_movies.cpp
    src/ui_movies/ui_movies.cpp
    src/ui_movies/keys.cpp
    src/ui_movies/animation.cpp
    src/ui_movies/track_properties.cpp
    src/ui_movies/recording.cpp
    src/ui_movies/draw.cpp
    src/ui_movies/files.cpp
    src/hooks/ui_movies_game.cpp
    src/ui_movies/game.cpp
    src/ui_movies/objects.cpp
    src/ui_movies/effect.cpp
    src/ui_movies/texts.cpp
    src/ui_movies/update.cpp
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
    src/car_physics/vehicle_update.cpp
    src/car_physics/vehicle_state.cpp
    src/car_physics/collision_world.cpp
    src/car_physics/car_data.cpp
    src/car_physics/car_setup.cpp
    src/car_physics/vehicle_physics.cpp
    src/hooks/vehicle_native.cpp
    src/vehicle/vehicle.cpp
    src/hooks/app_states.cpp
    src/app_states/containers.cpp
    src/app_states/app.cpp
    src/app_states/layers.cpp
    src/hooks/collision.cpp
    src/collision/shapes.cpp
    src/collision/boxes.cpp
    src/collision/shape_record.cpp
    src/collision/box_box.cpp
    src/collision/impacts.cpp
    src/collision/pairs.cpp
    src/hooks/collision_world.cpp
    src/track_collision/track_world.cpp
    src/track_collision/track_mesh.cpp
    src/track_collision/track_ground.cpp
    src/track_collision/track_cell.cpp
    src/track_collision/track_walls.cpp
    src/track_collision/track_wall_query.cpp
    src/hooks/track_route.cpp
    src/track_route/track_route.cpp
    src/hooks/ai_drivers.cpp
    src/ai_drivers/ai_drivers.cpp
    src/hooks/racer_controllers.cpp
    src/racer_controllers/racer_controllers.cpp
    src/hooks/track_zones.cpp
    src/track_zones/track_zones.cpp
    src/hooks/graphics_resources.cpp
    src/graphics/resources.cpp
    src/graphics/edram.cpp
    src/hooks/video_output.cpp
    src/graphics/video_output.cpp
    src/hooks/device_state.cpp
    src/graphics/device_state.cpp
    src/hooks/post_effects.cpp
    src/post_effects/post_effects.cpp
    src/hooks/framework.cpp
    src/framework/framework.cpp
    src/framework/run_loop.cpp
    src/framework/main_loop.cpp
    src/hooks/game_modes.cpp
    src/game_modes/game_modes.cpp
    src/game_modes/race_overlays_machine.cpp
    src/hooks/mode_tick.cpp
    src/hooks/minimap.cpp
    src/mode_tick/mode_tick.cpp
    src/minimap/minimap.cpp
    src/hooks/scenery_lists.cpp
    src/scenery_lists/scenery_lists.cpp
    src/scenery_lists/model_draws.cpp
    src/hooks/scene_models.cpp
    src/scene_models/scene_models.cpp
    src/hooks/models.cpp
    src/models/memory_blocks.cpp
    src/models/xpr.cpp
    src/models/texture_provider.cpp
    src/models/model.cpp
    src/models/model_load.cpp
    src/hooks/shader_objects.cpp
    src/shader_objects/shader_objects.cpp
    src/hooks/race_hud.cpp
    src/race_hud/race_hud.cpp
    src/hooks/race_setup.cpp
    src/race_setup/car.cpp
    src/race_setup/race_data.cpp
    src/race_setup/course_routes.cpp
    src/race_setup/set_up.cpp
    src/hooks/screen_objects.cpp
    src/screen_objects/uv_trees.cpp
    src/screen_objects/screen_objects.cpp
    src/hooks/course_scenery.cpp
    src/course_scenery/course_scenery.cpp
    src/course_scenery/collect.cpp
    src/hooks/action_mappers.cpp
    src/action_mappers/action_mappers.cpp
    src/hooks/data_tables.cpp
    src/data_tables/data_tables.cpp
    src/hooks/model_animation.cpp
    src/model_animation/model_animation.cpp
    src/hooks/car_specs.cpp
    src/car_specs/car_specs.cpp
    src/hooks/cameras.cpp
    src/cameras/cameras.cpp
    src/hooks/strings_and_font.cpp
    src/strings_and_font/strings_and_font.cpp
    src/strings_and_font/font.cpp
    src/hooks/std_string.cpp
    src/std_string/std_string.cpp
    src/hooks/sound_cues.cpp
    src/sound_cues/sound_cues.cpp
    src/hooks/xact.cpp
    src/xact/xact.cpp
    src/hooks/command_list.cpp
    src/command_list/command_list.cpp
    src/hooks/c_runtime.cpp
    src/hooks/math.cpp
    src/c_runtime/math.cpp
    src/c_runtime/strings.cpp
    src/c_runtime/format.cpp
    src/c_runtime/wide_format.cpp
    src/c_runtime/start_up.cpp
    src/c_runtime/parse.cpp
    src/c_runtime/arrays.cpp
    src/c_runtime/exit.cpp
    src/game_data/game_data.cpp
    src/game_data/data_file.cpp
    src/hooks/data_file.cpp
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
    src/harnesses/heap.cpp
    src/harnesses/c_runtime_heap.cpp
    src/harnesses/math.cpp
    src/harnesses/ui_movies.cpp
    src/harnesses/ui_movies_game.cpp
    src/harnesses/race_overlays.cpp
    src/harnesses/vehicle_accessors.cpp
    src/harnesses/matrix_math.cpp
    src/harnesses/car_physics.cpp
    src/harnesses/car_physics_objects.cpp
    src/harnesses/vehicle.cpp
    src/harnesses/app_states.cpp
    src/harnesses/collision.cpp
    src/harnesses/collision_world.cpp
    src/harnesses/collision_world_walls.cpp
    src/harnesses/track_route.cpp
    src/harnesses/ai_drivers.cpp
    src/harnesses/racer_controllers.cpp
    src/harnesses/track_zones.cpp
    src/harnesses/graphics_resources.cpp
    src/harnesses/video_output.cpp
    src/harnesses/device_state.cpp
    src/harnesses/course_scenery.cpp
    src/harnesses/post_effects.cpp
    src/harnesses/screen_objects.cpp
    src/harnesses/hardcoded_shaders.cpp
    src/harnesses/framework.cpp
    src/harnesses/game_modes.cpp
    src/harnesses/mode_tick.cpp
    src/harnesses/minimap.cpp
    src/harnesses/scene_models.cpp
    src/harnesses/scenery_lists.cpp
    src/harnesses/shader_objects.cpp
    src/harnesses/data_file.cpp
    src/harnesses/race_hud.cpp
    src/harnesses/race_setup.cpp
    src/harnesses/ui_movie_objects.cpp
    src/harnesses/models.cpp
    src/harnesses/action_mappers.cpp
    src/harnesses/data_tables.cpp
    src/harnesses/model_animation.cpp
    src/harnesses/car_specs.cpp
    src/harnesses/cameras.cpp
    src/harnesses/strings_and_font.cpp
    src/harnesses/std_string.cpp
    src/harnesses/sound_cues.cpp
    src/harnesses/xact.cpp
    src/harnesses/input.cpp
    src/harnesses/c_runtime_exit.cpp
    src/helpers/debug_output.cpp
    src/helpers/object_attributes.cpp
    src/helpers/time.cpp
    src/helpers/clock.cpp
    src/helpers/strings.cpp
    src/helpers/compare_string.cpp
    src/helpers/executable.cpp
    src/user/profile.cpp
    src/input/gamepads.cpp
    src/input/input_processor.cpp
    src/input/input_manager.cpp
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
    src/debug/shader_compile_dump.cpp
    src/hooks/shader_compiles.cpp
    src/hooks/movies.cpp
    src/movies/movie.cpp
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
# (src/ui_movies/keys.cpp) and its drawing walk (src/ui_movies/draw.cpp) and the
# game's texts drawn (src/ui_movies/texts.cpp), the full-frame effect's quad
# (src/ui_movies/effect.cpp) and the update's time steps
# (src/ui_movies/update.cpp) likewise (docs/functions/ui-movies.md), and the
# matrix math (src/matrix_math/matrix_math.cpp, docs/functions/matrix-math.md)
# and the car physics (src/car_physics/, docs/functions/car-physics.md) and
# the collision shapes (src/collision/, docs/functions/collision.md) and the
# track's collision (src/track_collision/, docs/functions/collision-world.md)
# and the model animation (src/model_animation/,
# docs/functions/model-animation.md) and the car specs (src/car_specs/,
# docs/functions/car-specs.md) and the cameras (src/cameras/,
# docs/functions/cameras.md) and the string table and font
# (src/strings_and_font/, docs/functions/strings-and-font.md) and the
# course's route (src/track_route/, docs/functions/track-route.md) and the AI
# drivers (src/ai_drivers/, docs/functions/ai-drivers.md) and the course
# scenery's collection and draws (src/course_scenery/collect.cpp,
# docs/functions/course-scenery.md) and the screen
# objects (src/screen_objects/, docs/functions/screen-objects.md) and the
# model draws (src/scenery_lists/model_draws.cpp,
# docs/functions/scenery-lists.md) and the model's load (its weights,
# src/models/model_load.cpp, docs/functions/models.md) and the race's set-up
# (src/race_setup/, docs/functions/race-setup.md) and the vehicle above the
# car physics (src/vehicle/, docs/functions/vehicle.md).
set_source_files_properties(src/c_runtime/math.cpp src/ui_movies/keys.cpp
    src/ui_movies/animation.cpp
    src/ui_movies/draw.cpp src/ui_movies/texts.cpp src/ui_movies/update.cpp
    src/ui_movies/effect.cpp
    src/matrix_math/matrix_math.cpp src/car_physics/rigid_body.cpp
    src/car_physics/vehicle_step.cpp src/car_physics/vehicle_update.cpp
    src/car_physics/vehicle_state.cpp src/car_physics/collision_world.cpp
    src/car_physics/car_data.cpp src/car_physics/car_setup.cpp
    src/car_physics/vehicle_physics.cpp src/collision/shapes.cpp
    src/collision/boxes.cpp src/collision/shape_record.cpp
    src/collision/box_box.cpp src/collision/impacts.cpp src/collision/pairs.cpp
    src/track_collision/track_world.cpp src/track_collision/track_mesh.cpp
    src/track_collision/track_ground.cpp src/track_collision/track_cell.cpp
    src/track_collision/track_walls.cpp src/track_collision/track_wall_query.cpp
    src/action_mappers/action_mappers.cpp src/model_animation/model_animation.cpp
    src/car_specs/car_specs.cpp src/cameras/cameras.cpp
    src/strings_and_font/strings_and_font.cpp src/strings_and_font/font.cpp
    src/track_route/track_route.cpp src/ai_drivers/ai_drivers.cpp
    src/screen_objects/screen_objects.cpp src/scenery_lists/model_draws.cpp
    src/models/model_load.cpp src/course_scenery/collect.cpp
    src/race_setup/car.cpp src/race_setup/race_data.cpp src/race_setup/course_routes.cpp
    src/race_setup/set_up.cpp src/vehicle/vehicle.cpp PROPERTIES
    COMPILE_OPTIONS "-ffp-model=strict;-ffp-contract=off"
    SKIP_PRECOMPILE_HEADERS ON)

# Our XMA decoding (src/audio/xma_*: B2 in docs/rexglue-removal.md) on our
# own FFmpeg, in a library of its own so only our FFmpeg's headers reach it.
add_library(itc_xma STATIC src/audio/xma_packets.cpp src/audio/xma_stream.cpp)
target_include_directories(itc_xma PUBLIC src)
target_link_libraries(itc_xma PRIVATE itc_avcodec)

# The movies played through our own FFmpeg (src/movies/: docs/native-movies.md),
# in a library of its own for the same reason.
add_library(itc_movies STATIC src/movies/player.cpp)
target_include_directories(itc_movies PUBLIC src)
target_link_libraries(itc_movies PRIVATE itc_avformat)

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
# The harnesses (src/harnesses/, --run_harnesses: "Checking with a harness" in
# docs/rexglue-removal.md) are for development: CI's builds, the releases'
# among them, leave them out (-DITC_HARNESSES=OFF; the user's call,
# 2026-10-25), and so does --run_harnesses with them.
option(ITC_HARNESSES "Build the harnesses (src/harnesses/, --run_harnesses)" ON)
if(NOT ITC_HARNESSES)
    list(FILTER ITC_SOURCES EXCLUDE REGEX "^src/harnesses/")
endif()

function(itc_add_executable name)
    if(WIN32)
        add_executable(${name} WIN32 ${ARGN})
    else()
        add_executable(${name} ${ARGN})
    endif()

    rexglue_setup_target(${name})

    target_include_directories(${name} PRIVATE src)
    if(NOT ITC_HARNESSES)
        target_compile_definitions(${name} PRIVATE ITC_NO_HARNESSES)
    endif()

    # Our own copies of header-only libraries (A1 in docs/rexglue-removal.md),
    # included with their folder (<xxHash/xxhash.h>) so the SDK's copies on the
    # include path can't stand in for them.
    target_include_directories(${name} PRIVATE thirdparty)

    target_link_libraries(${name} PRIVATE itc_xma itc_movies itc_xenos_shaders)

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

# Plays one movie through src/movies/ in real time (docs/native-movies.md; how
# to run it is at the top of the file). Not built by default.
add_executable(itc_movie_check EXCLUDE_FROM_ALL tests/movie_check.cpp)
target_link_libraries(itc_movie_check PRIVATE itc_movies)
