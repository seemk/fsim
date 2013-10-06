cmake_minimum_required (VERSION 2.8)
project (fsm)

if(UNIX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(GLFW_PATH "third_party/glfw")
add_subdirectory(${GLFW_PATH})

include_directories(${GLFW_PATH}/include)
include_directories(include)

add_subdirectory(src)
