# 包含各个模块的源文件
add_subdirectory(frontend)
add_subdirectory(backend)
add_subdirectory(utility)
add_subdirectory(IR)
add_subdirectory(IRPrinter)

# 编译器主驱动程序

add_executable(SysYCompiler main.cpp)
target_link_libraries(SysYCompiler PRIVATE
        AST
        Interpreter
        IR
        IREmitter
	InstSel
        IRPrinter
        utility
        argparse::argparse
)
