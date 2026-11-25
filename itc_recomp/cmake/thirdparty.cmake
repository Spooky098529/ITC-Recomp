# Our own third-party libraries (A1 and E4's step 5 in docs/rexglue-removal.md):
# submodules in itc_recomp/thirdparty/, pinned to the versions the ReXGlue SDK
# used, built with the options it set. The SDK itself isn't part of our build
# (only its shader translator, cmake/xenos_translator.cmake).

set(ITC_THIRDPARTY ${CMAKE_CURRENT_LIST_DIR}/../thirdparty)

foreach(lib SDL Vulkan-Headers simde glslang fmt)
    if(NOT EXISTS ${ITC_THIRDPARTY}/${lib}/CMakeLists.txt AND NOT EXISTS ${ITC_THIRDPARTY}/${lib}/simde)
        message(FATAL_ERROR "Submodule missing: git submodule update --init itc_recomp/thirdparty/${lib}")
    endif()
endforeach()

# SDL3: the window, events, gamepads and audio output. Static, with the
# subsystems the SDK turned off left off (no SDL renderer or OpenGL: our
# renderer makes its Vulkan surface through SDL).
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_INSTALL OFF CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_UNIX_CONSOLE_BUILD ON CACHE BOOL "" FORCE)
set(SDL_VIDEO ON CACHE BOOL "" FORCE)
set(SDL_VULKAN ON CACHE BOOL "" FORCE)
set(SDL_METAL OFF CACHE BOOL "" FORCE)
set(SDL_RENDER OFF CACHE BOOL "" FORCE)
set(SDL_HAPTIC OFF CACHE BOOL "" FORCE)
set(SDL_SENSOR OFF CACHE BOOL "" FORCE)
set(SDL_POWER OFF CACHE BOOL "" FORCE)
set(SDL_FILESYSTEM OFF CACHE BOOL "" FORCE)
set(SDL_LOCALE ON CACHE BOOL "" FORCE)
set(SDL_FILE OFF CACHE BOOL "" FORCE)
set(SDL_MISC OFF CACHE BOOL "" FORCE)
set(SDL_DIALOG ON CACHE BOOL "" FORCE)
set(SDL_TRAY OFF CACHE BOOL "" FORCE)
set(SDL_CAMERA OFF CACHE BOOL "" FORCE)
set(SDL_OPENGL OFF CACHE BOOL "" FORCE)
set(SDL_OPENGLES OFF CACHE BOOL "" FORCE)
set(SDL_DISKAUDIO OFF CACHE BOOL "" FORCE)
set(SDL_DUMMYAUDIO OFF CACHE BOOL "" FORCE)
set(SDL_DUMMYVIDEO OFF CACHE BOOL "" FORCE)
add_subdirectory(${ITC_THIRDPARTY}/SDL ${CMAKE_BINARY_DIR}/thirdparty/SDL EXCLUDE_FROM_ALL)

# The Vulkan headers (our renderer loads Vulkan at run time).
add_library(itc_vulkan_headers INTERFACE)
target_include_directories(itc_vulkan_headers SYSTEM INTERFACE ${ITC_THIRDPARTY}/Vulkan-Headers/include)

# simde (header-only): the vector helpers' portable intrinsics.
add_library(itc_simde INTERFACE)
target_include_directories(itc_simde SYSTEM INTERFACE ${ITC_THIRDPARTY}/simde)

# fmt: the shader translator's formatting.
set(FMT_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(${ITC_THIRDPARTY}/fmt ${CMAKE_BINARY_DIR}/thirdparty/fmt EXCLUDE_FROM_ALL)

# glslang: only its SPIR-V builder, which the shader translator writes with
# (as the SDK configured it).
set(ENABLE_GLSLANG_BINARIES OFF CACHE BOOL "" FORCE)
set(ENABLE_SPVREMAPPER OFF CACHE BOOL "" FORCE)
set(ENABLE_HLSL OFF CACHE BOOL "" FORCE)
set(ENABLE_OPT OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(GLSLANG_TESTS OFF CACHE BOOL "" FORCE)
set(SKIP_GLSLANG_INSTALL ON CACHE BOOL "" FORCE)
set(GLSLANG_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(ENABLE_RTTI ON CACHE BOOL "" FORCE)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)
add_subdirectory(${ITC_THIRDPARTY}/glslang ${CMAKE_BINARY_DIR}/thirdparty/glslang EXCLUDE_FROM_ALL)
if(NOT TARGET glslang::SPIRV)
    add_library(glslang::SPIRV ALIAS SPIRV)
endif()
