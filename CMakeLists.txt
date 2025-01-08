cmake_minimum_required(VERSION 3.16)

project(revcounter VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT DEFINED ENV{HADOOP_HOME})
    message(FATAL_ERROR "HADOOP_HOME environment variable is not set.")
endif()

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

set(HADOOP_INSTALL $ENV{HADOOP_HOME})
set(HADOOP_INCLUDE_DIR "${HADOOP_INSTALL}/include")
set(HADOOP_LIB_DIR "${HADOOP_INSTALL}/lib/native")

include_directories(${INCLUDE_DIR} ${HADOOP_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O2")
set(HADOOP_LIBS "-lhadooppipes -lhadooputils -lpthread -lssl -lcrypto -ltirpc")

#file(GLOB_RECURSE SRC_FILES ${SRC_DIR}/*.cc)

set(SRC_1 src/main.cc include/MapReduce.h src/MapReduce.cc)
set(SRC_2 src/main_agr.cc include/MapReduceAgr.h src/MapReduceAgr.cc)

#add_executable(${PROJECT_NAME} ${SRC_FILES})

add_executable(revcounter ${SRC_1})

add_executable(revagr ${SRC_2})

target_link_directories(revcounter PRIVATE ${HADOOP_LIB_DIR})
target_link_libraries(revcounter PRIVATE ${HADOOP_LIBS})

target_link_directories(revagr PRIVATE ${HADOOP_LIB_DIR})
target_link_libraries(revagr PRIVATE ${HADOOP_LIBS})
