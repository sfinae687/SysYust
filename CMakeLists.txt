cmake_minimum_required(VERSION 3.27)
project(SysYust VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

option(SELF_CONTAINS "let all dependency store in lib/" OFF)
option(GeneratedDocument "add a target to generate document by doxygen form code comments" ON)

include(FetchContent)

# 配置变量

set(DOXYGEN_CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
set(SYSYUST_PUBLIC_HEADER_DIR ${CMAKE_SOURCE_DIR}/include)

include_directories(include)

# 包含 Doxygen

find_package(Doxygen)

if (NOT DEFINED DOXYGEN_FOUND)
    message("Doxygen is not found")
endif ()

# 启用第三方库

# 包含GoogleTest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 添加 fmt
add_subdirectory(lib/fmt)

# 添加 argparse
FetchContent_Declare(
        argparse
        GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)

# 启用 ctest
enable_testing()
include(GoogleTest)
add_subdirectory(test)

# 添加子目录

add_subdirectory(src)

# 准备生成文档的目标

if (${GeneratedDocument} AND DEFINED DOXYGEN_FOUND)
    add_custom_target(Document
        COMMAND Doxygen::doxygen ${DOXYGEN_CONFIG_FILE}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif ()