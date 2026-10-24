# Upstream FFmpeg (itc_recomp/thirdparty/FFmpeg, a submodule pinned to a
# release) with only what the game's data needs: the xma2 decoder, for the
# game's XMA sounds (B2 in docs/rexglue-removal.md), and the asf demuxer with
# the wmv3 and wmav2 decoders, for its movies (docs/native-movies.md). Built from the files its
# configure made, kept in itc_recomp/thirdparty/ffmpeg-config/<platform>/ by
# tools/ffmpeg_config.sh (configure never runs here). Static libraries with
# hidden symbols: the SDK's own FFmpeg (Xenia's fork, in librexruntime) is
# separate and exports none of its symbols, so the two don't meet.
#
# Targets: itc_avutil, itc_avcodec (links itc_avutil) and itc_avformat
# (links itc_avcodec). Named apart from the SDK's libavutil and libavcodec
# targets.

enable_language(C)

set(ITC_FFMPEG_DIR ${CMAKE_CURRENT_LIST_DIR}/../thirdparty/FFmpeg)
if(WIN32)
    set(ITC_FFMPEG_PLATFORM windows-x86_64)
else()
    set(ITC_FFMPEG_PLATFORM linux-x86_64)
endif()
set(ITC_FFMPEG_CONFIG ${CMAKE_CURRENT_LIST_DIR}/../thirdparty/ffmpeg-config/${ITC_FFMPEG_PLATFORM})
if(NOT EXISTS ${ITC_FFMPEG_DIR}/configure)
    message(FATAL_ERROR "FFmpeg submodule missing: git submodule update --init itc_recomp/thirdparty/FFmpeg")
endif()

file(STRINGS ${ITC_FFMPEG_CONFIG}/sources.txt ITC_FFMPEG_SOURCES)
set(ITC_AVUTIL_SOURCES ${ITC_FFMPEG_SOURCES})
list(FILTER ITC_AVUTIL_SOURCES INCLUDE REGEX "^libavutil/")
set(ITC_AVCODEC_SOURCES ${ITC_FFMPEG_SOURCES})
list(FILTER ITC_AVCODEC_SOURCES INCLUDE REGEX "^libavcodec/")
set(ITC_AVFORMAT_SOURCES ${ITC_FFMPEG_SOURCES})
list(FILTER ITC_AVFORMAT_SOURCES INCLUDE REGEX "^libavformat/")
# The generated lists are #included from the config directory (allcodecs.c
# and friends), not compiled on their own.
list(FILTER ITC_AVCODEC_SOURCES EXCLUDE REGEX "_list\\.c$")
list(FILTER ITC_AVFORMAT_SOURCES EXCLUDE REGEX "_list\\.c$")
list(TRANSFORM ITC_AVUTIL_SOURCES PREPEND ${ITC_FFMPEG_DIR}/)
list(TRANSFORM ITC_AVCODEC_SOURCES PREPEND ${ITC_FFMPEG_DIR}/)
list(TRANSFORM ITC_AVFORMAT_SOURCES PREPEND ${ITC_FFMPEG_DIR}/)

add_library(itc_avutil STATIC ${ITC_AVUTIL_SOURCES})
add_library(itc_avcodec STATIC ${ITC_AVCODEC_SOURCES})
target_link_libraries(itc_avcodec PUBLIC itc_avutil)
add_library(itc_avformat STATIC ${ITC_AVFORMAT_SOURCES})
target_link_libraries(itc_avformat PUBLIC itc_avcodec)

foreach(target itc_avutil itc_avcodec itc_avformat)
    # As FFmpeg's own build: the generated files first, then the sources.
    target_include_directories(${target} BEFORE PUBLIC ${ITC_FFMPEG_CONFIG} ${ITC_FFMPEG_DIR})
    # configure's CPPFLAGS and CFLAGS (ffbuild/config.mak), without its warnings.
    target_compile_definitions(${target} PRIVATE
        HAVE_AV_CONFIG_H _ISOC11_SOURCE _FILE_OFFSET_BITS=64 _LARGEFILE_SOURCE PIC)
    target_compile_options(${target} PRIVATE
        -std=c17 -fomit-frame-pointer -fno-math-errno -fno-signed-zeros -w)
    if(WIN32)
        target_compile_definitions(${target} PRIVATE
            WIN32_LEAN_AND_MEAN _USE_MATH_DEFINES _CRT_SECURE_NO_WARNINGS
            _CRT_NONSTDC_NO_WARNINGS _WIN32_WINNT=0x0600)
        target_include_directories(${target} PRIVATE ${ITC_FFMPEG_DIR}/compat/stdbit)
    else()
        target_compile_definitions(${target} PRIVATE _POSIX_C_SOURCE=200112 _XOPEN_SOURCE=600)
        target_compile_options(${target} PRIVATE -pthread)
    endif()
    set_target_properties(${target} PROPERTIES
        C_VISIBILITY_PRESET hidden
        POSITION_INDEPENDENT_CODE ON)
endforeach()

# configure's EXTRALIBS for them.
if(WIN32)
    target_link_libraries(itc_avutil PUBLIC user32 bcrypt)
    target_link_libraries(itc_avcodec PUBLIC ole32 user32)
else()
    find_package(Threads REQUIRED)
    target_link_libraries(itc_avutil PUBLIC m Threads::Threads)
endif()
