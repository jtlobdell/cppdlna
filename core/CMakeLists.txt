cmake_minimum_required (VERSION 3.1)

set(PROJECT_NAME cppdlna)

project(${PROJECT_NAME})
set(EXECUTABLE_NAME ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)


# Find all the required external libs
find_package(Boost 1.70.0 REQUIRED COMPONENTS system filesystem log)

# Check for deviations from the happy path.
# Report each library that wasn't found.
if (NOT Boost_FOUND)
   message(SEND_ERROR "Could not find appropriate version of Boost.")
   set(ERROR_MISSING_LIB TRUE)
endif()

# If any libraries were not found then it's time to quit
if (ERROR_MISSING_LIB)
   message(FATAL_ERROR "One or more required packages not found.")
endif()

# Everything looks good, so proceed with generation.
include_directories(
	${Boost_INCLUDE_DIRS}
)

add_definitions(
	-Wall
	-Werror
	-Wfatal-errors
	-DBOOST_LOG_DYN_LINK
)

include_directories(inc)
add_subdirectory(src)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} pthread)
