cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(PROJECT_NAME Emulator6502)

set(CMAKE_CXX_COMPILER, "g++")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} 
	src/main.cpp
	src/asm.cpp
	include/asm.hpp
	src/cpu.cpp
	include/cpu.hpp
	src/memory.cpp
	include/memory.hpp	
	src/gui.cpp
	include/gui.hpp
	)

target_include_directories(${PROJECT_NAME} PUBLIC include)