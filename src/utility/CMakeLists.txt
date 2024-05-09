# 添加源文件列表
set(SYSYUST_UTILITY_SOURCE
    logger.cpp
)
add_library(utility
        ${SYSYUST_UTILITY_PUBLIC_HEADER}
        ${SYSYUST_UTILITY_SOURCE}
)
target_include_directories(utility PUBLIC ${SYSYUST_PUBLIC_HEADER_DIR}/utility)