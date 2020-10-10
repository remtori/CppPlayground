cmake_minimum_required(VERSION 3.16)

set(CMAKE_INSTALL_MESSAGE NEVER)

project(CppPlayground)

enable_testing()

function(make_bin target_name)
    add_executable(${target_name} ${SOURCES})
    install(TARGETS ${target_name} RUNTIME DESTINATION .)
endfunction()

function(make_lib target_name)
    add_library(${target_name} ${SOURCES}})
endfunction()

unset(CMAKE_SYSROOT)
set(CMAKE_STAGING_PREFIX ${CMAKE_SOURCE_DIR}/out)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/out)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-warning-option -Wall -Wextra -Werror -Wmissing-declarations -std=c++2a -fdiagnostics-color=always")
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconcepts")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -g1 -fno-exceptions -fno-rtti -Wno-address-of-packed-member -Wundef -Wcast-qual -Wwrite-strings -Wimplicit-fallthrough -Wno-nonnull-compare -Wno-deprecated-copy -Wno-expansion-to-defined")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")

include_directories(Libraries)

add_subdirectory(Applications)
add_subdirectory(Libraries)
