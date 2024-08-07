set(IRPRINTER_SOURCE
	IRPrinter.cpp
)

add_library(IRPrinter ${IRPRINTER_SOURCE})
target_include_directories(IRPrinter PUBLIC ${SYSYUST_PUBLIC_INCLUDE_DIR}/IRPrinter)
target_link_libraries(IRPrinter PUBLIC IR utility)