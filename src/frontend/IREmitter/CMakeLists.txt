set(IREMITTER_SOURCE
	IREmitter.cpp
)

add_library(IREmitter ${IREMITTER_SOURCE})
target_include_directories(IREmitter PUBLIC ${SYSYUST_PUBLIC_INCLUDE_DIR}/IREmitter)
