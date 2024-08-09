set(INSTSEL_SOURCE
	InstSel.cpp
)

add_library(InstSel ${INSTSEL_SOURCE})
target_include_directories(InstSel PUBLIC ${SYSYUST_PUBLIC_INCLUDE_DIR}/InstSel)
