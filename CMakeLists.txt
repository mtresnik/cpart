cmake_minimum_required(VERSION 3.5)
project(cpart)

set(CMAKE_CXX_STANDARD 26)

if (MSVC OR CMAKE_C_SIMULATE_ID STREQUAL "MSVC")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

include(CheckIncludeFileCXX)
check_include_file_cxx(filesystem HAVE_STD_FILESYSTEM)
check_include_file_cxx(experimental/filesystem HAVE_STD_EXPERIMENTAL_FILESYSTEM)
check_include_file_cxx(source_location HAVE_STD_LOCATION)
check_include_file_cxx(experimental/source_location HAVE_STD_EXPERIMENTAL_LOCATION)

set(DEPENDENCIES "${PROJECT_SOURCE_DIR}/libs")

execute_process(COMMAND git submodule update --init --recursive)

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.27)
    cmake_policy(SET CMP0148 OLD)
endif ()

add_subdirectory(libs/glfw)
add_subdirectory(libs/glad)

include_directories(libs/glfw/deps)
include_directories(libs/glm)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(FLAGS)
if (HAVE_STD_FILESYSTEM)
    list(APPEND FLAGS "-DSTD_FILESYSTEM")
elseif(HAVE_STD_EXPERIMENTAL_FILESYSTEM)
    MESSAGE(STATUS "Using include/experimental/filesystem")
else()
    list(APPEND FLAGS "-DFILESYSTEM_BOOST")
endif()
if (HAVE_STD_LOCATION)
    list(APPEND FLAGS "-DSTDLOCATION")
elseif(HAVE_STD_EXPERIMENTAL_LOCATION)
    list(APPEND FLAGS "-DLOCATION_EXPERIMENTAL")
else()
    list(APPEND FLAGS "-DLOCATION_NONE")
endif()
list(APPEND FLAGS "-lrt")
MESSAGE(STATUS ${FLAGS})

add_executable(cpart src/main.cpp)

set(CMAKE_CXX_FLAGS ${FLAGS})

set_target_properties(cpart PROPERTIES COMPILE_FLAGS ${FLAGS})
target_link_libraries(cpart PRIVATE glad glfw)