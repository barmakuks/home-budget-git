cmake_minimum_required(VERSION 2.8)
project(home-budget-qt)

find_package(Qt5Core)
find_package(Qt5Network)
find_package(Qt5Widgets)
find_package(Boost COMPONENTS REQUIRED)


add_definitions(${QT_DEFINITIONS})
add_definitions(-std=c++11)

set(GUI_ROOT ./src)
include_directories(${GUI_ROOT})
set(ICONS_RCS ./res/icons.qrc)
file(GLOB_RECURSE GUI_SRC ${GUI_ROOT}/*.cpp)
file(GLOB GUI_HEADERS ${GUI_ROOT}/*.h)
file(GLOB_RECURSE GUI_FORMS ${GUI_ROOT}/forms/*.ui)

qt5_wrap_ui(GENERATED_FORMS_HEADERS ${GUI_FORMS})
qt5_wrap_cpp(GENERATED_HEADERS_MOC ${GUI_HEADERS})
qt5_add_resources(ICONS_RC_SRCS ${ICONS_RCS})

include_directories(${HB_INCLUDES})
include_directories(${Boost_INCLUDE_DIR})
include_directories(${STORAGE_INCLUDES})
include_directories(${ENGINE_INCLUDES})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(home-budget-qt ${GUI_SRC} ${GENERATED_HEADERS_MOC} ${GENERATED_FORMS_HEADERS} ${ICONS_RC_SRCS} ${GUI_FORMS})
target_link_libraries(home-budget-qt sqlite-engine sql-storage home-budget ${CMAKE_DL_LIBS} Qt5::Widgets pthread)
