cmake_minimum_required(VERSION 3.28)
project(cpart)

set(CMAKE_CXX_STANDARD 26)

include(CheckIncludeFileCXX)
check_include_file_cxx(filesystem HAVE_STD_FILESYSTEM)
check_include_file_cxx(experimental/filesystem HAVE_STD_EXPERIMENTAL_FILESYSTEM)
check_include_file_cxx(source_location HAVE_STD_LOCATION)
check_include_file_cxx(experimental/source_location HAVE_STD_EXPERIMENTAL_LOCATION)

set(DEPENDENCIES "${PROJECT_SOURCE_DIR}/libs")
if(NOT EXISTS ${DEPENDENCIES})
    file(MAKE_DIRECTORY ${DEPENDENCIES})
endif ()

set(GLFW_LOCAL "${DEPENDENCIES}/glfw")
set(GLFW_LOCAL_GIT "${GLFW_LOCAL}/.git")

if(EXISTS ${GLFW_LOCAL} AND EXISTS ${GLFW_LOCAL_GIT})
    MESSAGE(STATUS "Found GLFW in directory:" ${GLFW_LOCAL})
else ()
    MESSAGE(STATUS "Cloning GLFW into directory:" ${GLFW_LOCAL})
    execute_process(COMMAND git clone https://github.com/glfw/glfw
            WORKING_DIRECTORY ${DEPENDENCIES})
endif ()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${GLFW_LOCAL})
set(GLFW_INCLUDE_DIR "${GLFW_LOCAL}/include")

FIND_PACKAGE(OpenGL REQUIRED)
MESSAGE(STATUS "OpenGL include directory: " "${OPENGL_INCLUDE_DIR}")
MESSAGE(STATUS "OpenGL library: " "${OPENGL_LIBRARY}")

set(FREE_GLUT_LOCAL "${DEPENDENCIES}/freeglut")
set(FREE_GLUT_LOCAL_GIT "${FREE_GLUT_LOCAL}/.git")

if (EXISTS ${FREE_GLUT_LOCAL} AND EXISTS ${FREE_GLUT_LOCAL_GIT})
    execute_process(COMMAND git pull
            WORKING_DIRECTORY ${FREE_GLUT_LOCAL})
else()
    execute_process(COMMAND git clone https://github.com/freeglut/freeglut.git
            WORKING_DIRECTORY ${DEPENDENCIES})
endif()
add_subdirectory(${FREE_GLUT_LOCAL})
set(FREE_GLUT_INCLUDE_DIR "${FREE_GLUT_LOCAL}/include")

set(GLM_LOCAL "${DEPENDENCIES}/glm")
set(GLM_LOCAL_GIT "${GLM_LOCAL}/.git")

if (EXISTS ${GLM_LOCAL} AND EXISTS ${GLM_LOCAL_GIT})
    execute_process(COMMAND git pull
            WORKING_DIRECTORY ${GLM_LOCAL})
else()
    execute_process(COMMAND git clone https://github.com/g-truc/glm
            WORKING_DIRECTORY ${DEPENDENCIES})
endif()
set(GLM_INCLUDE_DIRS "${GLM_LOCAL}/glm")

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
MESSAGE(STATUS ${FLAGS})

add_executable(cpart src/main.cpp)

target_include_directories(cpart PUBLIC "${PROJECT_SOURCE_DIR}/src" "${GLM_INCLUDE_DIRS}" "${GLFW_INCLUDE_DIR}")