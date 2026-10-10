# itc - ReXGlue Recompiled Project
#
# SDK boilerplate lives in generated/rexglue.cmake.

cmake_minimum_required(VERSION 3.25)
project(itc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(generated/rexglue.cmake)

# Sources
set(ITC_SOURCES
    src/main.cpp
)

if(WIN32)
    add_executable(itc WIN32 ${ITC_SOURCES})
else()
    add_executable(itc ${ITC_SOURCES})
endif()

rexglue_setup_target(itc GPU_PLUGINS xenos)

# rex::runtime doesn't propagate imgui's include path, which rex_app.cpp and
# the app header need. Headers only: the symbols already live in librexruntime.
target_include_directories(itc PRIVATE
    $<TARGET_PROPERTY:imgui::imgui,INTERFACE_INCLUDE_DIRECTORIES>)

# The SDK builds librexruntime.so into its own output dir. Stage it next to the
# executable, where its $ORIGIN rpath looks, so no LD_LIBRARY_PATH is needed.
if(UNIX AND NOT APPLE AND TARGET rexruntime)
    add_custom_command(TARGET itc POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:rexruntime> $<TARGET_FILE_DIR:itc>
        VERBATIM)
endif()
