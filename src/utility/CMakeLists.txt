# 添加源文件列表
set(SYSYUST_UTILITY_SOURCE
        Logger.cpp
        FileLogger.cpp
        StringLogger.cpp
        StreamLogger.cpp
)
add_library(utility
        ${SYSYUST_UTILITY_PUBLIC_HEADER}
        ${SYSYUST_UTILITY_SOURCE}
)
target_include_directories(utility
        PUBLIC ${SYSYUST_PUBLIC_HEADER_DIR}/utility
        ${CMAKE_SOURCE_DIR}/lib/expected/include
)

target_link_libraries(utility
        PUBLIC fmt::fmt
        PUBLIC range-v3::range-v3
)