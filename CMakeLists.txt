cmake_minimum_required (VERSION 2.8)
project (fsm)

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -ffast-math -std=c++11")
elseif(MSVC)
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")
	endif()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:fast")
	add_definitions(-DGLEW_STATIC)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(GLEW_PATH "third_party/glew")
set(GLFW_PATH "third_party/glfw")
set(GLM_PATH "third_party/glm")

add_subdirectory(${GLFW_PATH})

include_directories(${GLEW_PATH}/include)
include_directories(${GLFW_PATH}/include)
include_directories(${GLM_PATH}/)
include_directories(include)

add_subdirectory(${GLEW_PATH}/src)
add_subdirectory(src)