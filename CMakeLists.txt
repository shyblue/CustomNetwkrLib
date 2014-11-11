cmake_minimum_required(VERSION 2.8.4)
project(SkeletonServer)

set(tbb_DIR "D:/Work/libs/tbb")
set(BOOST_ROOT "D:/Work/libs/boost/1_55_0")
set(PSDK_DIR "D:/DevTool/PSDK")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D_WIN32_WINDOWS")
FIND_PACKAGE(Boost 1.55 REQUIRED)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} ${tbb_DIR}/include ${CMAKE_SOURCE_DIR} "D:/DevTool/MingW/include" )

file(GLOB SOURCE_FILES "sklt_server.cpp" "util/*.cpp" "db/*.cpp" "net/*.cpp" "handler/*.cpp")
file(GLOB HEADER_FILES "common.h" "util/*.h" "db/*.h" "network/*.h" "handler/*.h")
file(GLOB CUSTOM_RAND_FILES "rand/*.cpp" "rand/*.h" "rand/impl/*.cpp" "rand/impl/*.h")
file(GLOB PROTOCOLS "protocol/*.cpp" "protocol/*.h")
file(GLOB TESTS "test/*.h" "test/*.cpp")

add_executable(skltServer ${SOURCE_FILES} ${HEADER_FILES} ${CUSTOM_RAND_FILES} ${PROTOCOLS} ${TESTS})
TARGET_LINK_LIBRARIES( skltServer ${Boost_LIBRARIES} )