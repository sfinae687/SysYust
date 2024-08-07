cmake_minimum_required(VERSION 3.16)
project(SysYust VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

option(SELF_CONTAINS "let all dependency store in lib/" OFF)
option(GeneratedDocument "add a target to generate document by doxygen form code comments" OFF)
option(EnableASTBuildTest "enable the tests of AST Build" OFF)
option(EnableInterpreterTest "enable the tests of Interpreter" OFF)
option(EnableIREmitterBuildTest "enable the build tests of IREmitter" ON)
option(EnableIREmitterTest "enable the tests of IREmitter" OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


include(FetchContent)

# 配置变量

set(DOXYGEN_CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
set(SYSYUST_PUBLIC_HEADER_DIR ${CMAKE_SOURCE_DIR}/include)

set(sy_source_root ${CMAKE_SOURCE_DIR}/compiler2023/TestCase/functional)
set(sy_lib ${CMAKE_SOURCE_DIR}/compiler2023/TestCase/sylib.c)
set(sy_header ${CMAKE_SOURCE_DIR}/compiler2023/TestCase/sylib.h)
file(GLOB_RECURSE sy_source_list  ${sy_source_root}/*.sy)

set(ScriptDir ${CMAKE_SOURCE_DIR}/script)

find_package(Python COMPONENTS Interpreter)

include_directories(include src/frontend/generated)

# 包含 Doxygen

find_package(Doxygen)

if (NOT DEFINED DOXYGEN_FOUND)
    message("Doxygen is not found")
else()
    message(Doxygen:${DOXYGEN_EXECUTABLE})
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
add_subdirectory(lib/argparse)

# 添加 antlr4-cpp-runtime
set(ANTLR_BUILD_CPP_TESTS OFF)
add_subdirectory(lib/antlr4-cpp-runtime)

# 添加 range-v3
add_subdirectory(lib/range-v3)

# 启用 ctest
enable_testing()
include(GoogleTest)
add_subdirectory(test)

# 添加本项目的模块

add_subdirectory(${ScriptDir})
add_subdirectory(include)
add_subdirectory(src)

# 准备生成文档的目标

if (${GeneratedDocument} AND DEFINED DOXYGEN_FOUND)
    set(DocumentRoot ${CMAKE_BINARY_DIR}/doc CACHE STRING "The directory used to storage document")
    configure_file(${DOXYGEN_CONFIG_FILE} Doxyfile)
    add_custom_target(Document
        COMMAND Doxygen::doxygen ${CMAKE_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif ()
