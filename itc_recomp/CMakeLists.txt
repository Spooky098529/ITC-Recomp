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

include(generated/rexglue.cmake)

# Sources
set(ITC_SOURCES
    src/main.cpp
    src/hooks/vehicle.cpp
    src/frame_pacing/frame_pacer.cpp
    src/frame_pacing/renderer_hooks.cpp
    src/debug/state_capture.cpp
    src/debug/frame_state_diff.cpp
    src/interpolation/transform.cpp
    src/frame_pacing/game_messages.cpp
    src/frame_pacing/per_frame_logic.cpp
    src/interpolation/blend_state.cpp
)

if(WIN32)
    add_executable(itc WIN32 ${ITC_SOURCES})
else()
    add_executable(itc ${ITC_SOURCES})
endif()

rexglue_setup_target(itc GPU_PLUGINS xenos)

target_include_directories(itc PRIVATE src)

# rex::runtime doesn't propagate imgui's include path, which rex_app.cpp and
# the app header need. Headers only: the symbols already live in librexruntime.
target_include_directories(itc PRIVATE
    $<TARGET_PROPERTY:imgui::imgui,INTERFACE_INCLUDE_DIRECTORIES>)
