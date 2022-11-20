cmake_minimum_required( VERSION 3.21 )

project( metal_demo VERSION 1.0.0 )

option( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set( CMAKE_CXX_STANDARD          20   )
set( CMAKE_CXX_STANDARD_REQUIRED True )

add_compile_options( -Wall -Wextra -pedantic -O3 )

include_directories( include )

file( GLOB SOURCES "include/*.cpp" )

add_executable( metal_demo main.cpp ${SOURCES} )

target_link_libraries(
        metal_demo
        PUBLIC
        "-framework Metal"
        "-framework MetalKit"
        "-framework Foundation"
)
