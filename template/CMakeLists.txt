cmake_minimum_required(VERSION 2.8)
project(template)

set( CMAKE_BUILD_TYPE "Debug" )
set( CMAKE_CXX_COMPLIER "g++" )
set( CMAKE_CXX_FLAGS "-std=c++11 -march=native -O3")

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib )


add_subdirectory( src )
add_subdirectory( test )
