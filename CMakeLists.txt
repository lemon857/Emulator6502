cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(PROJECT_NAME Emulator6502)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} 
	src/main.cpp
	src/asm.cpp
	include/asm.hpp
	src/cpu.cpp
	include/cpu.hpp
	src/memory.cpp
	include/memory.hpp	
	)

target_include_directories(${PROJECT_NAME} PUBLIC include)