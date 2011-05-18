cmake_minimum_required (VERSION 2.8)
project (PDAEMON)

file(GLOB pdaemon_SOURCES
"*.c" "*.h")

option(SIMULATION "Use the simulation" ON)

if (${SIMULATION})
	add_definitions("-DSIMULATION=1")
	set(CMAKE_BUILD_TYPE "Debug")
else ()
	set(CMAKE_BUILD_TYPE "Release")
endif ()

add_executable(PDAEMON ${pdaemon_SOURCES})

message("")
message("-- Summary:")
message("* Simulation mode: ${SIMULATION}")
message("")