# CMakeLists.txt for valChess
# 12-01-2025 by madpl

cmake_minimum_required(VERSION 3.22)
project(valChess)

add_definitions(-Os -march=native -std=c++17)

file(GLOB_RECURSE SOURCE ./source/*)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	set(SFML_DIR "C:/Progs/SFML2.5.1SEH/lib/cmake/SFML")
endif()

find_package(SFML 2.5 REQUIRED COMPONENTS
	system window graphics)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE})
target_link_libraries(${CMAKE_PROJECT_NAME}
	sfml-system sfml-window sfml-graphics)
