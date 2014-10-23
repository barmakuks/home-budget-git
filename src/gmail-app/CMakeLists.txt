project(gmail-app)
cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_CXX_FLAGS "-std=c++11")
add_definitions(-g -O0 -Wall)

#************************** Mail API library ***************************************
set(MAIL_LIB_NAME mail)

include_directories(./mail-api/)
file(GLOB MAIL_HEADERS ./mail-api/*.h)
aux_source_directory(./mail-api/ MAIL_SRC)
add_library(${MAIL_LIB_NAME} STATIC ${MAIL_SRC} ${MAIL_HEADERS})

#************************** GMail API lib ******************************************
set(GMAIL_LIB_NAME gmail)
find_package(Boost COMPONENTS python REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# get python
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
link_directories(${PYTHON_LIBRARIES})

include_directories(./mail-api-gmail/)
aux_source_directory(./mail-api-gmail/ GMAIL_SRC)
set(GMAIL_PYTHON_SCRIPT ./mail-api-gmail/gmail-api.py)
set(GMAIL_CLIENT_SECRET ./mail-api-gmail/client_secret-home.budget.001.json)
add_library(${GMAIL_LIB_NAME} SHARED ${GMAIL_SRC} ${GMAIL_PYTHON_SCRIPT})
target_link_libraries(${GMAIL_LIB_NAME} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} )

add_custom_target(CopyScriptsTarget
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GMAIL_PYTHON_SCRIPT} $<TARGET_FILE_DIR:${GMAIL_LIB_NAME}>/gmail-api.py
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GMAIL_CLIENT_SECRET} $<TARGET_FILE_DIR:${GMAIL_LIB_NAME}>/client_secret-home.budget.001.json
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    COMMENT "Copy Python script file...")

#************************** Main Application ***************************************

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${MAIL_LIB_NAME} ${GMAIL_LIB_NAME} pthread)
add_dependencies(${PROJECT_NAME} CopyScriptsTarget)

