project(home-budget)
cmake_minimum_required(VERSION 2.8.1)
enable_testing(true)

#TODO separate include directories by executable targets
include_directories(./inc ./src/core ./src/storage ./src/storage/filters ./src/storage/strategies ./src/utils .src/gui)
# TODO setup link directory for unit tests only
#link_directories(${CMAKE_SOURCE_DIR}/../thirdparty/gtest/lib)

find_package(Boost COMPONENTS REQUIRED)

add_definitions(-g -std=c++11 -O0)

# SQLite library
set(SQLITE_LIB_NAME sqlite-lib)
aux_source_directory(./src/sqlite SQLITE_LIB_SRC)
add_library(${SQLITE_LIB_NAME} STATIC ${SQLITE_LIB_SRC} )

# TinyXML2 library
include_directories(./thirdparty/tinyxml2-master)
set(XML_LIB_NAME tinyxml2-lib)
aux_source_directory(./thirdparty/tinyxml2-master XML_LIB_SRC)
add_library(${XML_LIB_NAME} STATIC ${XML_LIB_SRC} )

#Main library
set(CORE_LIB_NAME core-lib)
file(GLOB_RECURSE CORE_HEADERS ./src/*.h)
file(GLOB_RECURSE CORE_TEMPLATES ./src/*.hpp)
add_subdirectory(thirdparty/tinyxml2-master)
aux_source_directory(./src/core CORE_LIB_SRC)
aux_source_directory(./src/storage CORE_LIB_SRC)
aux_source_directory(./src/storage/strategies CORE_LIB_SRC)
aux_source_directory(./src/storage/filters CORE_LIB_SRC)
aux_source_directory(./src/utils CORE_LIB_SRC)

add_library(${CORE_LIB_NAME} STATIC ${CORE_LIB_SRC} ${CORE_HEADERS} ${CORE_TEMPLATES})
target_link_libraries(${CORE_LIB_NAME} ${Boost_LIBRARIES} ${XML_LIB_NAME})

#Executable console
set(MAIN_SRC ./src/main.cpp)
add_executable(${PROJECT_NAME} ${MAIN_SRC})
target_link_libraries(${PROJECT_NAME} ${SQLITE_LIB_NAME} ${CORE_LIB_NAME} ${XML_LIB_NAME} ${Boost_LIBRARIES} ${CMAKE_DL_LIBS} pthread)

#GUI application
FIND_PACKAGE(Qt4 COMPONENTS QtMain QtGui QtCore QtNetwork REQUIRED)
set(GUI_PROJECT_NAME home-budget-gui)
set(GUI_MAIN_SRC ./src/gui-main.cpp)
aux_source_directory(./src/web QT_WEB_SRC)
set(ICONS_RCS ./res/icons.qrc)
file(GLOB_RECURSE GUI_SRC src/gui/*.cpp)
file(GLOB GUI_HEADERS src/gui/*.h)
file(GLOB WEB_HEADERS src/gui/web/*.h)
file(GLOB_RECURSE GUI_FORMS src/gui/forms/*.ui)

qt4_wrap_ui(GENERATED_FORMS_HEADERS ${GUI_FORMS})
qt4_wrap_cpp(GENERATED_HEADERS_MOC ${GUI_HEADERS})
qt4_wrap_cpp(GENERATED_WEB_HEADERS_MOC ${WEB_HEADERS})
qt4_add_resources(ICONS_RC_SRCS ${ICONS_RCS})

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(${GUI_PROJECT_NAME} ${GUI_MAIN_SRC} ${GUI_SRC} ${QT_WEB_SRC} ${GENERATED_HEADERS_MOC} ${GENERATED_WEB_HEADERS_MOC} ${GENERATED_FORMS_HEADERS} ${ICONS_RC_SRCS})
target_link_libraries(${GUI_PROJECT_NAME} ${SQLITE_LIB_NAME} ${CORE_LIB_NAME}  ${Boost_LIBRARIES} ${CMAKE_DL_LIBS} ${QT_LIBRARIES} pthread)

#Tests
include_directories(./thirdparty/gtest/include)
add_subdirectory(./thirdparty/gtest)
link_directories(./thirdparty/gtest/lib)
set(TEST_EXE_NAME home-budget-test)
aux_source_directory(./tests TEST_SRC)
add_executable(${TEST_EXE_NAME} EXCLUDE_FROM_ALL ${TEST_SRC})
target_link_libraries(${TEST_EXE_NAME} gtest gtest_main  ${Boost_LIBRARIES} ${CORE_LIB_NAME} ${CMAKE_THREAD_LIBS_INIT})

add_custom_target(test
                  COMMAND ./${TEST_EXE_NAME})

add_dependencies(test ${TEST_EXE_NAME})
