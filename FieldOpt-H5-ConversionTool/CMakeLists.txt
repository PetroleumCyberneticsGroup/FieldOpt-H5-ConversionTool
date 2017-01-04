cmake_minimum_required(VERSION 3.5)

# PROJECT
project(H5_ConversionTool VERSION 0.1 LANGUAGES CXX)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(${CMAKE_SOURCE_DIR})

# HDF5
find_package(HDF5 REQUIRED COMPONENTS CXX)
add_definitions(${HDF5_DEFINITIONS})

# BOOST
find_package(Boost REQUIRED)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif ()
#set(Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
#set(Boost_USE_MULTITHREAD ON) # enable multithreading
#find_package(Boost COMPONENTS REQUIRED program_options system filesystem)

# FIELDOPT: HDF5 SUMMARYREADER HEADER FILES
set(FIELDOPT_DIR ${PROJECT_SOURCE_DIR}/../../FieldOpt/FieldOpt)
set(HDF5READER_DIR ${FIELDOPT_DIR}/Hdf5SummaryReader)
set(HDF5READER_HEADER_FILES ${HDF5READER_DIR}/hdf5_summary_reader.h)

# FIELDOPT: BUILD DIRECTORY
set(FIELDOPT_BUILD ${PROJECT_SOURCE_DIR}/../../FieldOpt/build-FieldOpt-debug)

# OPM
set(OPM_DIR ../../OPM)
set(OPM_HEADER_FILES
        ${OPM_DIR}/output/eclipse/EclipseWriter.hpp
        ${OPM_DIR}/output/eclipse/EclipseReader.hpp
        ${OPM_DIR}/parser/eclipse/Parser/ParseContext.hpp)

# ERT LIBRARIES
find_library(ert_ecl NAMES ecl REQUIRED)
find_library(ert_util NAMES ert_util REQUIRED)

# ERT
#set(ERT_DIR ../../OPM/ert)
#set(ERT_HEADER_FILES ${ERT_DIR}/ecl/ecl_util.h)
#include_directories(${ERT_DIR})

# ALL INCLUDE DIRECTORIES
include_directories(
        ${HDF5_INCLUDE_DIRS}
        ${HDF5READER_DIR}
        ${OPM_DIR})

# COMPILE
set(SOURCE_FILES main.cpp)
set(HEADER_FILES ${HDF5READER_HEADER_FILES} ${ERT_HEADER_FILES})
#set(HEADER_FILES )

add_executable(H5_ConversionTool ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(H5_ConversionTool
        PUBLIC ${FIELDOPT_BUILD}/lib/libhdf5summaryreader.a
        PUBLIC ${HDF5_LIBRARIES}
        )


