project(home-budget)
cmake_minimum_required(VERSION 2.8)

#TODO separate include directories by executable targets
include_directories(./inc ./src/core ./src/storage ./src/storage/filters ./src/storage/strategies)
# TODO setup link directory for unit tests only
#link_directories(${CMAKE_SOURCE_DIR}/../thirdparty/gtest/lib)

add_definitions(-g -std=c++11 -O0)

# Main library
set(SQLITE_LIB_NAME sqlite-lib)
aux_source_directory(./src/sqlite SQLITE_LIB_SRC)
add_library(${SQLITE_LIB_NAME} STATIC ${SQLITE_LIB_SRC})

#Main library
set(CORE_LIB_NAME core-lib)
aux_source_directory(./src/core CORE_LIB_SRC)
aux_source_directory(./src/storage CORE_LIB_SRC)
aux_source_directory(./src/storage/strategies CORE_LIB_SRC)
aux_source_directory(./src/storage/filters CORE_LIB_SRC)

add_library(${CORE_LIB_NAME} STATIC ${CORE_LIB_SRC})

#Executable
set(MAIN_SRC ./src/main.cpp)
add_executable(${PROJECT_NAME} ${MAIN_SRC})
target_link_libraries(${PROJECT_NAME} ${SQLITE_LIB_NAME} ${CORE_LIB_NAME} ${CMAKE_DL_LIBS} pthread)

#Tests
#set(TEST_EXE_NAME task-test)
#aux_source_directory(./test TEST_SRC)
#add_executable(${TEST_EXE_NAME} EXCLUDE_FROM_ALL ${TEST_SRC})
#target_link_libraries(${TEST_EXE_NAME} gtest gtest_main ${MAIN_LIB_NAME} ${UTIL_LIB_NAME} ${CMAKE_THREAD_LIBS_INIT})

#add_custom_target(test
#                  COMMAND ./${TEST_EXE_NAME})

#add_dependencies(test ${TEST_EXE_NAME})
