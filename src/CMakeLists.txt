set(fsm_HEADERS
	${fsm_SOURCE_DIR}/include/array2d.hpp
	${fsm_SOURCE_DIR}/include/material.hpp
	${fsm_SOURCE_DIR}/include/particle.hpp
	${fsm_SOURCE_DIR}/include/fluidsimulator.hpp
	)

set(fsm_SOURCES
	main.cpp
	fluidsimulator.cpp
	)

add_executable(fluid ${fsm_SOURCES} ${fsm_HEADERS})
target_link_libraries(fluid
	glfw ${GLFW_LIBRARIES})