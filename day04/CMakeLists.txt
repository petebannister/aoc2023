cmake_minimum_required(VERSION 3.10)
project(day04)
set(CMAKE_CXX_STANDARD 20)
add_executable(day04
	"${CMAKE_CURRENT_LIST_DIR}/main.cpp" 
	)
target_compile_definitions(day04 PUBLIC -D_SECURE_SCL=0)
target_include_directories(day04 PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(day04 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../common)
configure_file("${CMAKE_CURRENT_LIST_DIR}/../launch.vs.json.in" "${CMAKE_CURRENT_LIST_DIR}/.vs/launch.vs.json" @ONLY)
