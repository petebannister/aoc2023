cmake_minimum_required(VERSION 3.10)
project(aoc)
set(CMAKE_CXX_STANDARD 20)
add_executable(aoc 
	"${CMAKE_CURRENT_LIST_DIR}/main.cpp" 
	)
target_compile_definitions(aoc PUBLIC -D_SECURE_SCL=0)
target_include_directories(aoc PUBLIC ${CMAKE_CURRENT_LIST_DIR})

#configure_file("${CMAKE_CURRENT_LIST_DIR}/example.txt" "${CMAKE_BINARY_DIR}/"  COPYONLY)
#configure_file("${CMAKE_CURRENT_LIST_DIR}/input.txt" "${CMAKE_BINARY_DIR}/"  COPYONLY)
#configure_file("${CMAKE_CURRENT_LIST_DIR}/input.txt" "${CMAKE_BINARY_DIR}/Debug"  COPYONLY)
#configure_file("${CMAKE_CURRENT_LIST_DIR}/input.txt" "${CMAKE_BINARY_DIR}/Release"  COPYONLY)
set_property(TARGET aoc PROPERTY VS_DEBUGGER_WORKING_DIRECTORY 
	"$<TARGET_FILE_DIR:aoc>"
	VS_DEBUGGER_COMMAND "$<TARGET_FILE:aoc>"
	VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${CMAKE_CURRENT_LIST_DIR}")