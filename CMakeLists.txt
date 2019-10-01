cmake_minimum_required(VERSION 3.0)

project(sudoku_validator C)

set(SOURCES
	main.c
	thread.c
	reader.c
	)

set(HEADERS
	thread.h
	reader.h
	parameter.h
	)

find_package (Threads)

add_executable(sudoku ${SOURCES} ${HEADERS})

target_link_libraries (sudoku ${CMAKE_THREAD_LIBS_INIT})
