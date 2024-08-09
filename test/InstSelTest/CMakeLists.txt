# InstSelTest

# 自动测试

add_executable(InstSelTest InstSelTest.cpp)
target_link_libraries(InstSelTest PRIVATE InstSel IREmitter)

list(LENGTH sy_source_list sy_length)
if (${EnableInstSelBuildTest} AND NOT ${sy_length} EQUAL 0)
    list(LENGTH sy_source_list sy_length)
    MATH(EXPR sy_length "${sy_length}-1")
    foreach(index RANGE ${sy_length})
        list(GET sy_test_name_list ${index} current_name)
        list(GET sy_source_list ${index} current_path)
        list(GET sy_test_exe_list ${index} sy_executable)
#        message(${current_name}:${current_path})
        add_test(NAME InstSel_Build_${current_name}
            COMMAND InstSelTest ${current_path}
        )
    endforeach ()
endif()
