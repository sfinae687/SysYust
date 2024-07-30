set(IR_source
        Code.cpp
        Procedure.cpp
        ControlFlow.cpp
        BasicBlock.cpp
        Instruction.cpp
        TypeUtil.cpp
        SymbolUtil.cpp
        ProcedureContext.cpp
        CodeContext.cpp
        InstructionUtil.cpp
        CodeUtil.cpp
        ContextualMixin.cpp
        InitInfo.cpp
        Def-Use/ProcedureDU.cpp
        Def-Use/DU_Util.cpp
)
add_library(IR ${IR_source})
target_link_libraries(IR utility)