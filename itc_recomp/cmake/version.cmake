# Our version string (in place of the SDK's build stamp): `git describe` of
# this repo, for the window's title.
execute_process(
    COMMAND git describe --tags --always --dirty
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../..
    OUTPUT_VARIABLE ITC_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE ITC_VERSION_RESULT ERROR_QUIET)
if(NOT ITC_VERSION_RESULT EQUAL 0 OR ITC_VERSION STREQUAL "")
    set(ITC_VERSION "unknown")
endif()
file(CONFIGURE OUTPUT ${CMAKE_BINARY_DIR}/itc_include/itc_version.h CONTENT
"// Written by cmake/version.cmake at configure time.
#pragma once
#define ITC_VERSION \"@ITC_VERSION@\"
" @ONLY)
