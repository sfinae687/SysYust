set(AST_SOURCE
        TypeBase.cpp
        Int.cpp
        Float.cpp
        Void.cpp
        Array.cpp
        Pointer.cpp
        Function.cpp
)

add_library(AST ${AST_SOURCE})
target_link_libraries(AST PUBLIC utility)
target_include_directories(AST PUBLIC ${SYSYUST_PUBLIC_INCLUDE_DIR}/AST)