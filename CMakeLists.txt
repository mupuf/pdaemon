cmake_minimum_required (VERSION 2.8)
INCLUDE(FindPkgConfig)
project (PDAEMON)

file(GLOB pdaemon_SOURCES
"*.c" "*.h")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

option(SIMULATION "Use the simulation" ON)
if (${SIMULATION})
	file(GLOB pdaemon_simul_SOURCES "simul/*.c" "simul/*.h")
	add_definitions("-DSIMULATION=1")
	set(CMAKE_BUILD_TYPE "Debug")

	PKG_SEARCH_MODULE(pciaccess pciaccess)
	if(pciaccess_FOUND)
		include_directories(${pciaccess_INCLUDE_DIRS})
		link_directories(${pciaccess_LIBRARY_DIRS})
	endif(pciaccess_FOUND)
else ()
	set(CMAKE_BUILD_TYPE "Release")
endif ()

add_executable(PDAEMON ${pdaemon_SOURCES} ${pdaemon_simul_SOURCES})
target_link_libraries(PDAEMON ${pciaccess_LIBRARIES})

message("")
message("-- Summary:")
message("* Simulation mode: ${SIMULATION}")
message("")