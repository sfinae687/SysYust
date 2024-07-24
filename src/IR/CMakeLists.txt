set(IR_source
        Code.cpp
        Procedure.cpp
        ControlFlow.cpp
        BasicBlock.cpp
        Instruction.cpp
        TypeUtil.cpp
        InstructionUtil.cpp
)
add_library(IR ${IR_source})
target_link_libraries(IR utility)