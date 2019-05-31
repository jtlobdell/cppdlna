add_subdirectory(net)
add_subdirectory(config)
add_subdirectory(log)
add_subdirectory(upnp)

set(SOURCES
	${SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	PARENT_SCOPE
)
