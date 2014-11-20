cmake_minimum_required(VERSION 2.8.4)
project(SkeletonServer)

set(tbb_DIR "D:/Work/libs/tbb")
set(BOOST_ROOT "D:/Work/libs/boost/1_55_0")
set(PSDK_DIR "D:/DevTool/PSDK")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D_WIN32_WINDOWS")
FIND_PACKAGE(Boost 1.55 REQUIRED)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} ${tbb_DIR}/include ${CMAKE_SOURCE_DIR}/include "D:/DevTool/MingW/include" )

#file(GLOB HEADER_FILES "include/*.h" "include/util/*.h" "incluyde/db/*.h" "include/network/*.h" "include/handler/*.h")
file(GLOB LIB_SRCS "src/lib/*.cpp")
file(GLOB SERVER_SRCS "src/dummyServer/*.cpp")
file(GLOB CLIENT_SRCS "src/testClient/*.cpp")

add_library(sklt ${LIB_SRCS})
add_executable(dummyServer ${SERVER_SRCS} )
#add_executable(testClient ${CLIENT_SRCS} )