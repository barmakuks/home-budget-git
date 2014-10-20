project(home-budget)
cmake_minimum_required(VERSION 2.8.11)
enable_testing(true)

#set(CMAKE_PREFIX_PATH $ENV{QTDIR})

find_package(Qt5Core)
find_package(Qt5Network)
find_package(Qt5Widgets)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOMOC ON)

find_package(Boost COMPONENTS REQUIRED)
add_definitions(${QT_DEFINITIONS})

set(CMAKE_CXX_FLAGS "-std=c++11")
add_definitions(-g -O0 -Wall)

############################################################ SQLite library ###############################################################
include_directories(./src/sqlite)
set(SQLITE_LIB_NAME sqlite-lib)
aux_source_directory(./src/sqlite SQLITE_LIB_SRC)
add_library(${SQLITE_LIB_NAME} STATIC ${SQLITE_LIB_SRC} )

############################################################ TinyXML2 library #############################################################
include_directories(./thirdparty/tinyxml2-master)
set(XML_LIB_NAME tinyxml2-lib)
aux_source_directory(./thirdparty/tinyxml2-master XML_LIB_SRC)
add_library(${XML_LIB_NAME} STATIC ${XML_LIB_SRC} )

############################################################ Web Services library #########################################################
include_directories(./src/web ./src/utils)
set(WEB_LIB_NAME web-services-lib)
aux_source_directory(./src/web WEB_LIB_SRC)
aux_source_directory(./src/utils UTILS_SRC)
file(GLOB WEB_HEADERS ./src/web/*.h)
qt5_wrap_cpp(GENERATED_WEB_HEADERS_MOC ./src/web/qt-web-engine.h)
add_library(${WEB_LIB_NAME} STATIC ${WEB_LIB_SRC} ${UTILS_SRC} ${GENERATED_WEB_HEADERS_MOC} )
target_link_libraries(${WEB_LIB_NAME} ${XML_LIB_NAME} Qt5::Network)

############################################################ Core library #################################################################
include_directories(./inc ./src/core ./src/storage ./src/storage/filters ./src/storage/strategies ./src/utils ./src/currency-manager)
set(CORE_LIB_NAME core-lib)
file(GLOB_RECURSE CORE_HEADERS ./src/*.h)
file(GLOB_RECURSE CORE_TEMPLATES ./src/*.hpp)
add_subdirectory(thirdparty/tinyxml2-master)
aux_source_directory(./src/core CORE_LIB_SRC)
aux_source_directory(./src/currency-manager CUR_LIB_SRC)
aux_source_directory(./src/storage CORE_LIB_SRC)
aux_source_directory(./src/storage/strategies CORE_LIB_SRC)
aux_source_directory(./src/storage/filters CORE_LIB_SRC)

add_library(${CORE_LIB_NAME} STATIC ${CORE_LIB_SRC} ${CUR_LIB_SRC} ${CORE_HEADERS} ${CORE_TEMPLATES} ${UTILS_SRC})
target_link_libraries(${CORE_LIB_NAME} ${Boost_LIBRARIES} ${XML_LIB_NAME} Qt5::Core)

############################################################ GUI application ##############################################################
include_directories(./src/gui)
set(GUI_MAIN_SRC ./src/gui-main.cpp)
set(ICONS_RCS ./res/icons.qrc)
file(GLOB_RECURSE GUI_SRC src/gui/*.cpp)
file(GLOB GUI_HEADERS src/gui/*.h)
file(GLOB_RECURSE GUI_FORMS src/gui/forms/*.ui)

qt5_wrap_ui(GENERATED_FORMS_HEADERS ${GUI_FORMS})
qt5_wrap_cpp(GENERATED_HEADERS_MOC ${GUI_HEADERS})
qt5_add_resources(ICONS_RC_SRCS ${ICONS_RCS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${GUI_MAIN_SRC} ${GUI_SRC} ${GENERATED_HEADERS_MOC} ${GENERATED_FORMS_HEADERS} ${ICONS_RC_SRCS})
target_link_libraries(${PROJECT_NAME} ${SQLITE_LIB_NAME} ${CORE_LIB_NAME} ${WEB_LIB_NAME} ${Boost_LIBRARIES} ${CMAKE_DL_LIBS} Qt5::Widgets pthread)

############################################################ Test executable console ######################################################
set(TEST_APP_NAME home-budget-test-app)
aux_source_directory(./src/test_app TEST_APP_MAIN_SRC)
add_executable(${TEST_APP_NAME} ${TEST_APP_MAIN_SRC})
target_link_libraries(${TEST_APP_NAME} ${SQLITE_LIB_NAME} ${CORE_LIB_NAME} ${XML_LIB_NAME} ${WEB_LIB_NAME} ${Boost_LIBRARIES} ${CMAKE_DL_LIBS} Qt5::Widgets pthread)

############################################################ Tests ########################################################################
include_directories(./thirdparty/gtest/include)
add_subdirectory(./thirdparty/gtest)
link_directories(./thirdparty/gtest/lib)
set(TEST_EXE_NAME google-test)
aux_source_directory(./tests TEST_SRC)
add_executable(${TEST_EXE_NAME} EXCLUDE_FROM_ALL ${TEST_SRC})
target_link_libraries(${TEST_EXE_NAME} gtest gtest_main  ${Boost_LIBRARIES} ${CORE_LIB_NAME} ${CMAKE_THREAD_LIBS_INIT})

add_custom_target(test
                  COMMAND ./${TEST_EXE_NAME})

add_dependencies(test ${TEST_EXE_NAME})
