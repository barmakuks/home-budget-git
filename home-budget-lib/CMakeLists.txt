cmake_minimum_required(VERSION 2.8)
project(home-budget)

find_package(Boost COMPONENTS REQUIRED)

add_definitions(-std=c++11)

set(HB_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "hb:includes" FORCE)

include_directories(${HB_INCLUDES})
file(GLOB_RECURSE SRC src/*.cpp src/*.h src/*.hpp)
file(GLOB_RECURSE HEADERS ${HB_INCLUDES} *.h)
add_library(home-budget SHARED ${SRC} ${HEADERS})
target_link_libraries(home-budget ${Boost_LIBRARIES} ${XML_LIB_NAME})
