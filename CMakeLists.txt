project(CitySkyLine)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
include_directories(include)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_executable(${PROJECT_NAME} ${SRC_LIST})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET(EXTRA_LIBS ${EXTRA_LIBS} GLEW GLU GL)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(WIN32)
    SET(EXTRA_LIBS ${EXTRA_LIBS} glew32 glu32 opengl32 )
endif(WIN32)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} glfw ${EXTRA_LIBS} )
