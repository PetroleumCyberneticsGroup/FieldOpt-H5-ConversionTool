cmake_minimum_required(VERSION 3.5)
project(H5_ConversionTool)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Boost libraries
find_package(Boost REQUIRED)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif ()
#set(Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
#set(Boost_USE_MULTITHREAD ON) # enable multithreading
#find_package(Boost COMPONENTS REQUIRED program_options system filesystem)

set(SOURCE_FILES main.cpp)
add_executable(H5_ConversionTool ${SOURCE_FILES})