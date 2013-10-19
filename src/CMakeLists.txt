
file(GLOB_RECURSE fsm_HEADERS ${fsm_SOURCE_DIR}/include/*.hpp)
file(GLOB_RECURSE fsm_SOURCES *.cpp)

add_executable(${TARGET_NAME} ${fsm_SOURCES} ${fsm_HEADERS})
target_link_libraries(fluid
	glfw ${GLFW_LIBRARIES}
	glew ${GLEW_LIBRARIES})