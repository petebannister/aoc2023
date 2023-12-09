
option(AOC_DAY "AOC Day number" "dayXX")

configure_file("${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt.in" "${CMAKE_CURRENT_LIST_DIR}/../@AOC_DAY@/CMakeLists.txt" @ONLY)
configure_file("${CMAKE_CURRENT_LIST_DIR}/CMakeSettings.json.in" "${CMAKE_CURRENT_LIST_DIR}/../@AOC_DAY@/CMakeSettings.json" @ONLY)
configure_file("${CMAKE_CURRENT_LIST_DIR}/template.main.cpp" "${CMAKE_CURRENT_LIST_DIR}/../@AOC_DAY@/main.cpp" @ONLY)
