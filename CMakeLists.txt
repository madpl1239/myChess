# CMakeLists.txt for myChess
# 12-01-2025 by madpl

cmake_minimum_required(VERSION 3.31)
set(CMAKE_CXX_STANDARD_REQUIRED 23)
project(myChess)

add_definitions(-Os -march=native -std=c++23 -pthread)
file(GLOB_RECURSE SOURCE ./source/*)

option(WINDOWS "build for windows?" OFF)
if(WINDOWS)
	set(SFML_DIR "C:/Progs/SFML2.5.1SEH/lib/cmake/SFML")
else()
	find_package(SFML 2.5 REQUIRED COMPONENTS system window graphics audio)
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
	-lpthread sfml-system sfml-window sfml-graphics sfml-audio)
