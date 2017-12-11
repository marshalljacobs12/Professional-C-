# If you create new headers/cpp files, add them to these list!
set(HEADER_FILES
	catch.hpp
)

set(SOURCE_FILES
	Catch.cpp
	StudentTests.cpp
	GradedTests.cpp   
)

# Don't change this
add_executable(tests ${SOURCE_FILES})
target_link_libraries(tests src ${wxWidgets_LIBRARIES})

if (WIN32)
   add_custom_command(TARGET tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${wxWidgets_LIB_DIR}/wxmsw30u_core_vc140_x64.dll"
        $<TARGET_FILE_DIR:tests>)
   add_custom_command(TARGET tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${wxWidgets_LIB_DIR}/wxbase30u_vc140_x64.dll"
        $<TARGET_FILE_DIR:tests>)
endif()
