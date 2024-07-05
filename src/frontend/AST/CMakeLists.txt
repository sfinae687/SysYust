set(AST_SOURCE
        Type/TypeBase.cpp
        Type/Int.cpp
        Type/Float.cpp
        Type/Void.cpp
        Type/Array.cpp
        Type/Pointer.cpp
        Type/Function.cpp
        VarInfo.cpp
        FuncInfo.cpp
        Env.cpp
        NodeBase.cpp
        Decl.cpp
        NodeExecutor.cpp
        VarDecl.cpp
        FuncDecl.cpp
        ParamDecl.cpp
        Stmt.cpp
        Expr.cpp
        IntLiteral.cpp
        FloatLiteral.cpp
        Call.cpp
        UnaryOp.cpp
        BinaryOp.cpp
        LExpr.cpp
        DeclRef.cpp
        ArrayRef.cpp
        ToInt.cpp
        ToFloat.cpp
        CondExpr.cpp
        Not.cpp
        And.cpp
        Or.cpp
        Compare.cpp
        ToCond.cpp
        If.cpp
        Assign.cpp
        While.cpp
        Break.cpp
        Continue.cpp
        Return.cpp
        Empty.cpp
        Block.cpp
)

add_library(AST ${AST_SOURCE})
target_link_libraries(AST PUBLIC utility)
target_include_directories(AST PUBLIC ${SYSYUST_PUBLIC_INCLUDE_DIR}/AST)