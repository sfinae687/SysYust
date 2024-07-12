# 包含各个模块的源文件
add_subdirectory(frontend)
add_subdirectory(utility)

# 对所有的测试样例进行语法树构建测试
add_executable(AST_Builder ASTBuilder.cpp)
target_link_libraries(AST_Builder utility AST)


foreach (sy_source_path ${sy_source_list})
    STRING(REPLACE "/" "_" TestName ${sy_source_path})
    STRING(REGEX REPLACE "(.*)\.sy" "\\1" TestName ${TestName})
    add_test(NAME ${TestName}_Build_Test
            COMMAND AST_Builder ${sy_source_root}/${sy_source_path}
    )
endforeach ()