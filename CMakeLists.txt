cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(Emulate6502)

add_executable(Emulate6502 
	src/main.cpp
	src/asm.cpp
	include/asm.h
	src/cpu.cpp
	include/cpu.h
	src/memory.cpp
	include/memory.h	
	)

target_include_directories(Emulate6502 PUBLIC include)