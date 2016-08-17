cmake_minimum_required(VERSION 2.8)
project(sqlite-engine)

add_definitions(-std=c++11)

set(ENGINE_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "engine:includes" FORCE)

file(GLOB_RECURSE ENGINE_LIB_HEADERS ${ENGINE_INCLUDES}/*.h ${ENGINE_INCLUDES}/*.hpp)
file(GLOB_RECURSE ENGINE_LIB_SRC src/*.cpp thirdparty/*.h thirdparty/*.cpp thirdparty/*.c)

include_directories(${HB_INCLUDES})
include_directories(${ENGINE_INCLUDES})

add_library(sqlite-engine SHARED ${ENGINE_LIB_SRC} ${ENGINE_LIB_HEADERS})
