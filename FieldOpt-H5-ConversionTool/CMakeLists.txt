cmake_minimum_required(VERSION 3.5)

# PROJECT
project(H5_ConversionTool VERSION 0.1 LANGUAGES CXX)

# ============================================================
# ASSIGN CMAKE_RUNTIME AND CMAKE_LIBRARY OUTPUT DIRECTORIES

# BUILD/LIB DIRS =======================================================
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY: " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# CMAKE FLAGS ==========================================================
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(${CMAKE_SOURCE_DIR})

# HDF5 -================================================================
find_package(HDF5 REQUIRED COMPONENTS CXX)
add_definitions(${HDF5_DEFINITIONS})

# BOOST ================================================================
find_package(Boost REQUIRED)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif ()
set(Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
set(Boost_USE_MULTITHREAD ON) # enable multithreading
find_package(Boost REQUIRED COMPONENTS program_options 
                    system filesystem regex date_time)

# FIELDOPT: HDF5 SUMMARYREADER HEADER FILES ============================
set(FIELDOPT_DIR ${PROJECT_SOURCE_DIR}/../../FieldOpt/FieldOpt)
set(FIELDOPT_LIB ${FIELDOPT_DIR}/cmake-build-debug/lib)
set(HDF5READER_DIR ${FIELDOPT_DIR}/Hdf5SummaryReader)
set(HDF5READER_HEADER_FILES ${HDF5READER_DIR}/hdf5_summary_reader.h)

# OPM ==================================================================
set(OPM_DIR ${PROJECT_SOURCE_DIR}/../../../OPM)
set(OPM_HEADER_FILES
        ${OPM_DIR}/opm-output/opm/output/eclipse/EclipseWriter.hpp
        ${OPM_DIR}/opm-parser/opm/parser/eclipse/Parser/Parser.hpp
        ${OPM_DIR}/opm-parser/opm/parser/eclipse/Parser/ParseContext.hpp)

# ERT LIBRARIES ========================================================
set(ERT_STANDARD /usr/local/ert_standard/lib/x86_64-linux-gnu/)
set(ERT_STATOIL ${PROJECT_SOURCE_DIR}/../../../OPM/libecl-statoil/cmake-build-debug/lib/)

find_library(ert_ecl NAMES ecl REQUIRED)
find_library(ert_eclxx NAMES eclxx REQUIRED)
find_library(ert_well NAMES ecl_well REQUIRED)
find_library(ert_geometry NAMES ert_geometry REQUIRED)
find_library(ert_util NAMES ert_util REQUIRED)
find_library(ert_utilxx NAMES ert_utilxx REQUIRED)

# ALL INCLUDE DIRECTORIES ==============================================
include_directories(
        ${HDF5_INCLUDE_DIRS}
        ${HDF5READER_DIR}
        ${OPM_DIR}
        ${ERT_DIR})

# HEADER/SOURCE FILES ==================================================
set(SOURCE_FILES
        main.cpp
        src/H5Conv.cpp)
set(HEADER_FILES
        src/H5Conv.h
        src/colors.hpp
        ${OPM_HEADER_FILES}
        ${HDF5READER_HEADER_FILES}
        ${ERT_HEADER_FILES})

# EXECUTABLE TARGET ====================================================
add_executable(H5-ConversionTool ${SOURCE_FILES} ${HEADER_FILES})

# LINK LIBRARIES =======================================================
 target_link_libraries(H5-ConversionTool
         # FIELDOPT::HDF5SUMMARYREADER
         PUBLIC ${FIELDOPT_LIB}/libhdf5summaryreader.a
         # H5
         PUBLIC ${HDF5_LIBRARIES}
         # OPM
#         PUBLIC /usr/local/lib/x86_64-linux-gnu/libopmcommon.a
#         PUBLIC /usr/local/lib/x86_64-linux-gnu/libopmoutput.a
#         PUBLIC /usr/local/lib/x86_64-linux-gnu/libopmparser.a
#         PUBLIC /usr/local/lib/x86_64-linux-gnu/libopmjson.a
         #
         PUBLIC ${OPM_DIR}/build-opm-common/lib/libopmcommon.a
         PUBLIC ${OPM_DIR}/build-opm-output/lib/libopmoutput.a
         PUBLIC ${OPM_DIR}/build-opm-parser/lib/x86_64-linux-gnu/libopmparser.a
         PUBLIC ${OPM_DIR}/build-opm-parser/lib/x86_64-linux-gnu/libopmjson.a
         # BOOST
         PUBLIC ${Boost_LIBRARIES}
         # ERT
#         PUBLIC ${ERT_STANDARD}/libecl.so
#         PUBLIC ${ERT_STANDARD}/libeclxx.so
#         PUBLIC ${ERT_STANDARD}/libecl_well.so
#         PUBLIC ${ERT_STANDARD}/libert_geometry.so
#         PUBLIC ${ERT_STANDARD}/libert_util.so
#         PUBLIC ${ERT_STANDARD}/libert_utilxx.so
         PUBLIC ${ert_ecl}
         PUBLIC ${ert_eclxx}
         PUBLIC ${ert_well}
         PUBLIC ${ert_geometry}
         PUBLIC ${ert_util}
         PUBLIC ${ert_utilxx}
         )

## LINK LIBRARIES (KEEP FOR REFERENCE)
#target_link_libraries(H5ConversionTool
#        # FIELDOPT::HDF5SUMMARYREADER
#        PUBLIC ${FIELDOPT_BUILD}/lib/libhdf5summaryreader.a
#        # H5
#        PUBLIC ${HDF5_LIBRARIES}
#        # OPM
#        "/usr/local/lib/x86_64-linux-gnu/libopmcommon.a"
#        "/usr/local/lib/x86_64-linux-gnu/libopmparser.a"
#        "/usr/local/lib/x86_64-linux-gnu/libopmjson.a"
#        # BOOST
#        "/usr/lib/x86_64-linux-gnu/libboost_filesystem.so"
#        "/usr/lib/x86_64-linux-gnu/libboost_date_time.so"
#        "/usr/lib/x86_64-linux-gnu/libboost_system.so"
#        "/usr/lib/x86_64-linux-gnu/libboost_regex.so"
#        # ERT
#        "/usr/local/lib/x86_64-linux-gnu/libecl.so"
#        "/usr/local/lib/x86_64-linux-gnu/libeclxx.so"
#        "/usr/local/lib/x86_64-linux-gnu/libecl_well.so"
#        "/usr/local/lib/x86_64-linux-gnu/libert_geometry.so"
#        "/usr/local/lib/x86_64-linux-gnu/libert_util.so"
#        "/usr/local/lib/x86_64-linux-gnu/libert_utilxx.so"
#        # BLAS/LAPACK
#        "/usr/lib/x86_64-linux-gnu/libz.so;/usr/lib/liblapack.so"
#        "/usr/lib/libblas.so;/usr/lib/x86_64-linux-gnu/libm.so"
#        "-lpthread;dl"
#        )

target_compile_definitions(H5-ConversionTool
        PRIVATE PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

# LIBRARY TARGET
#add_library(h5conversiontool ${SOURCE_FILES} ${HEADER_FILES})
#target_link_libraries(h5conversiontool
#        PUBLIC ${FIELDOPT_BUILD}/lib/libhdf5summaryreader.a
#        PUBLIC ${HDF5_LIBRARIES})
#target_compile_definitions(h5conversiontool
#        PRIVATE PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

# ============================================================
#COPY FILES
file(COPY ${CMAKE_SOURCE_DIR}/example
        DESTINATION ${CMAKE_BINARY_DIR}/)

# ============================================================
# UNIT TESTS
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)
find_package(Boost REQUIRED) # 1.63
find_package(Boost COMPONENTS
        REQUIRED program_options system filesystem)

include_directories()
set(Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
set(Boost_USE_MULTITHREAD ON)  # enable multithreading

include(CTest)
enable_testing()
include_directories(${GTEST_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
        tests)

# Change output dir for test executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#add_executable(test_H5_ConversionTool
#        tests/test_H5_ConversionTool.cpp
#        tests/test_H5_ConversionTool.h)
#
#target_link_libraries(test_H5_ConversionTool
#        ${Boost_LIBRARIES} ${GTEST_LIBRARIES} pthread )
#
#add_test(NAME test_H5_ConversionTool
#        COMMAND $<TARGET_FILE:test_H5_ConversionTool>)