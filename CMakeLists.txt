cmake_minimum_required(VERSION 3.16)

set(CMAKE_INSTALL_MESSAGE NEVER)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 20)

project(CppPlayground)

function(make_bin target_name)
    add_executable(${target_name} ${SOURCES})
    install(TARGETS ${target_name} RUNTIME DESTINATION out)
endfunction()

function(make_lib target_name)
    add_library(${target_name} ${SOURCES}})
endfunction()

unset(CMAKE_SYSROOT)
set(CMAKE_STAGING_PREFIX ${CMAKE_SOURCE_DIR}/root)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/root)

include_directories(Libraries)

add_subdirectory(Applications)
add_subdirectory(Libraries)
