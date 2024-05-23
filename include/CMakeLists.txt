# 设置各个模块的公开头文件列表
file(GLOB utility_header ${CMAKE_CURRENT_SOURCE_DIR}/utility/*.h)
set(SYSYUST_UTILITY_PUBLIC_HEADER ${utility_header} PARENT_SCOPE)
