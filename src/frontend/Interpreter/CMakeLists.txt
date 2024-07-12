set(INTERPRETER_SOURCE
	Interpreter.cpp
	Value.cpp
	sylib.cpp
)

add_library(Interpreter ${INTERPRETER_SOURCE})
target_include_directories(Interpreter PUBLIC ${SYSYUST_PUBLIC_INCLUDE_DIR}/Interpreter)
