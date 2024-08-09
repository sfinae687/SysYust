set(DOMTREE_SOURCE
	DomTree.cpp
)

add_library(DomTree ${DOMTREE_SOURCE})
target_include_directories(DomTree PUBLIC ${SYSYUST_PUBLIC_INCLUDE_DIR}/DomTree)
