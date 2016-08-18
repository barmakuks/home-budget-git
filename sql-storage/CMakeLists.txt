cmake_minimum_required(VERSION 2.8)
project(sql-storage)

find_package(Boost COMPONENTS REQUIRED)

add_definitions(-std=c++11)

set(STORAGE_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "storage:includes" FORCE)

file(GLOB_RECURSE STORAGE_LIB_HEADERS ${STORAGE_INCLUDES}/*.h ${STORAGE_INCLUDES}/*.hpp)
file(GLOB_RECURSE STORAGE_LIB_SRC src/*.cpp src/*.h src/*.hpp ./src/strategies/*.cpp ./src/strategies/*.h ./src/filters/*.h)

include_directories(${HB_INCLUDES} ${Boost_INCLUDE_DIR})
include_directories(${STORAGE_INCLUDES})

add_library(sql-storage SHARED ${STORAGE_LIB_SRC} ${STORAGE_LIB_HEADERS})
target_link_libraries(sql-storage home-budget)
