# The ReXGlue SDK for our build: found (or built from its source tree) and
# set up on a target. Our own copy of what generated/rexglue.cmake did,
# without the recompiled code: generated/ isn't built since the rewiring's
# stage 5 (docs/rexglue-removal.md, "Before E1: the rewiring"), and nothing
# runs codegen.

# SDK version
# Set REXSDK_VERSION to pin a specific SDK version.
set(REXSDK_VERSION "" CACHE STRING "Override SDK version (leave empty for default)")

# Find SDK
set(REXSDK_DIR "" CACHE PATH "Path to rexglue-sdk source tree")
if(REXSDK_DIR)
    add_subdirectory("${REXSDK_DIR}" rexglue-sdk)
    message(STATUS "Using ReXGlue SDK from source tree: ${REXSDK_DIR}")
else()
    if(REXSDK_VERSION)
        find_package(rexglue ${REXSDK_VERSION} EXACT QUIET CONFIG)
    else()
        find_package(rexglue 0.10.0 QUIET CONFIG)
    endif()
    if(NOT rexglue_FOUND)
        message(FATAL_ERROR
            "ReXGlue SDK not found. Either:\n"
            "  - Set REXSDK_DIR to the rexglue-sdk source tree (e.g. thirdparty/rexglue-sdk)\n"
            "  - Install the SDK package and ensure it is on CMAKE_PREFIX_PATH")
    endif()
    message(STATUS "Found ReXGlue SDK ${REXGLUE_VERSION_STRING} at ${rexglue_DIR}")
endif()

if(NOT DEFINED REXGLUE_HOST_TARGET)
    set(REXGLUE_HOST_TARGET ${PROJECT_NAME})
endif()

# Configure a target with the SDK's libraries and platform settings.
# Call after add_executable(). Usage: rexglue_setup_target(<target>)
macro(rexglue_setup_target target_name)
    target_include_directories(${target_name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(${target_name} PRIVATE rex::runtime)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/metadata/icons")
        rexglue_embed_metadata(${target_name}
            DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/metadata/icons"
            PREFIX "icons")
    endif()
    rexglue_configure_target(${target_name} ${ARGN})
endmacro()
