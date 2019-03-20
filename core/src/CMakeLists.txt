add_subdirectory(net)
add_subdirectory(config)
add_subdirectory(log)

set(SOURCES
	${SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	PARENT_SCOPE
)
