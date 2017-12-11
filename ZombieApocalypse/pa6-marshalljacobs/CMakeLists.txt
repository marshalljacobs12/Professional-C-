# Require CMAKE 3.1 or higher
cmake_minimum_required(VERSION 3.1)

# Project name
project(pa6)

# Build for C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Turn on any compiler-specific flags
if (WIN32)
	# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
else()
	# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
   if ("$ENV{RELEASE}" STREQUAL "ON")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
	endif()
	if ("$ENV{SANITIZE}" STREQUAL "ON")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
	endif()
endif()

# Try to find wxWidgets
set(WX_ROOT_DIR "C:/wxWidgets-3.0.3" CACHE PATH "WX_ROOT_DIR")
set(wxWidgets_CONFIGURATION mswu)
set(wxWidgets_ROOT_DIR "C:/wxWidgets-3.0.3" CACHE PATH "wxWidgets_ROOT_DIR")
set(wxWidgets_LIB_DIR "C:/wxWidgets-3.0.3/lib/vc140_x64_dll" CACHE PATH "wxWidgets_LIB_DIR")
find_package(wxWidgets REQUIRED core base)
include(${wxWidgets_USE_FILE})

# Any source files in this directory
set(SOURCE_FILES Main.cpp)

# Where any include files are
include_directories(src)
if (WIN32)
include_directories("C:/wxWidgets-3.0.3/include")
endif()
include_directories(${wxWidgets_INCLUDE_DIRS})
# Subdirectories to build
add_subdirectory(src)
add_subdirectory(tests)

# Add executable
if (WIN32)
	add_executable(main WIN32 ${SOURCE_FILES} Main.rc)
else()
	add_executable(main ${SOURCE_FILES})
endif()

# Link main vs the source library
target_link_libraries(main src ${wxWidgets_LIBRARIES})

# Stupid stuff
if (WIN32)
   add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${wxWidgets_LIB_DIR}/wxmsw30u_core_vc140_x64.dll"
        $<TARGET_FILE_DIR:main>)
   add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${wxWidgets_LIB_DIR}/wxbase30u_vc140_x64.dll"
        $<TARGET_FILE_DIR:main>)
endif()
