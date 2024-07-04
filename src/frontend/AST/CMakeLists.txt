set(AST_SOURCE
        Type/TypeBase.cpp
        Type/Int.cpp
        Type/Float.cpp
        Type/Void.cpp
        Type/Array.cpp
        Type/Pointer.cpp
        Type/Function.cpp
        VarInfo.cpp
        Type/FuncInfo.cpp
        Env.cpp
)

add_library(AST ${AST_SOURCE})
target_link_libraries(AST PUBLIC utility)
target_include_directories(AST PUBLIC ${SYSYUST_PUBLIC_INCLUDE_DIR}/AST)