# 包含各个模块的源文件
add_subdirectory(frontend)
add_subdirectory(utility)

# 驱动
add_executable(AST_Builder ASTBuilder.cpp)
target_link_libraries(AST_Builder utility AST)