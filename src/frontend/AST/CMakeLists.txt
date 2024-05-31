set(AST_SOURCE
        Type.cpp
        Int.cpp
        Float.cpp
        Void.cpp
)

add_library(AST ${AST_SOURCE})
target_include_directories(AST PUBLIC ${SYSYUST_PUBLIC_INCLUDE_DIR}/AST)