project(home-budget)
cmake_minimum_required(VERSION 2.8.11)
enable_testing(true)

add_subdirectory(home-budget-lib)
add_subdirectory(sql-storage)
add_subdirectory(sqlite-engine)
add_subdirectory(qt-gui)

############################################################# TinyXML2 library #############################################################
#include_directories(./thirdparty/tinyxml2-master)
#set(XML_LIB_NAME tinyxml2-lib)
#aux_source_directory(./thirdparty/tinyxml2-master XML_LIB_SRC)
#add_library(${XML_LIB_NAME} STATIC ${XML_LIB_SRC} )

############################################################# Web Services library #########################################################
#include_directories(./src/web ./src/utils)
#set(WEB_LIB_NAME web-services-lib)
#aux_source_directory(./src/web WEB_LIB_SRC)
#aux_source_directory(./src/utils UTILS_SRC)
#file(GLOB WEB_HEADERS ./src/web/*.h)
#qt5_wrap_cpp(GENERATED_WEB_HEADERS_MOC ./src/web/qt-web-engine.h)
#add_library(${WEB_LIB_NAME} STATIC ${WEB_LIB_SRC} ${UTILS_SRC} ${GENERATED_WEB_HEADERS_MOC} )
#target_link_libraries(${WEB_LIB_NAME} ${XML_LIB_NAME} Qt5::Network)

############################################################ Test executable console ######################################################
#set(TEST_APP_NAME home-budget-test-app)
#aux_source_directory(./src/test_app TEST_APP_MAIN_SRC)
#add_executable(${TEST_APP_NAME} ${TEST_APP_MAIN_SRC})
#target_link_libraries(${TEST_APP_NAME} ${SQLITE_LIB_NAME} ${CORE_LIB_NAME} ${XML_LIB_NAME} ${WEB_LIB_NAME} ${Boost_LIBRARIES} ${CMAKE_DL_LIBS} Qt5::Widgets pthread)

############################################################ Tests ########################################################################
#include_directories(./thirdparty/gtest/include)
#add_subdirectory(./thirdparty/gtest)
#link_directories(./thirdparty/gtest/lib)
#set(TEST_EXE_NAME unit_tests)
#aux_source_directory(./tests TEST_SRC)
#add_executable(${TEST_EXE_NAME} EXCLUDE_FROM_ALL ${TEST_SRC})
#target_link_libraries(${TEST_EXE_NAME} gtest gtest_main  ${Boost_LIBRARIES} ${CORE_LIB_NAME} ${CMAKE_THREAD_LIBS_INIT})

#add_custom_target(run-test
#                  COMMAND ./${TEST_EXE_NAME})

#add_dependencies(run-test ${TEST_EXE_NAME})
