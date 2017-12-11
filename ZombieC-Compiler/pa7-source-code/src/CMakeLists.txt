# Flex/Bison
# On Windows, just assume these paths
if (WIN32)
	set(BISON_EXECUTABLE "C:/cygwin/bin/bison.exe")
	set(FLEX_EXECUTABLE "C:/cygwin/bin/flex.exe")
endif()

# Find packages
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

# Setup bison/flex targets
BISON_TARGET(MyParser Zompiler.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp COMPILE_FLAGS "-d -b zompiler -p zompiler")
FLEX_TARGET(MyScanner Zompiler.l  ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp COMPILE_FLAGS "-Pzompiler -Cem")
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

# If you create new headers/cpp files, add them to these list!
set(HEADER_FILES
	Node.h
	SrcMain.h
)

set(SOURCE_FILES
	Node.cpp
	SrcMain.cpp
)

# Don't change this
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_library(src ${SOURCE_FILES} ${HEADER_FILES}
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
)
