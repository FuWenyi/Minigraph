cmake_minimum_required(VERSION 2.8)

project(minigraph)
set(MINIGRAPH_COPYRIGHT_YEAR "2022")

###### Cmake options ######
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pthread")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Set default cmake type to Debug
if (NOT CMAKE_BUILD_TYPE)
    # cmake default flags with relwithdebinfo is -O2 -g
    # cmake default flags with release is -O3 -DNDEBUG
    set(CMAKE_BUILD_TYPE "Debug")
endif ()
message(STATUS "[MiniGraph] will build in type: ${CMAKE_BUILD_TYPE}")


###### Directories shortcuts ######
set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(THIRD_PARTY_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_ROOT_DIR}/bin)


###### Add third party to the project, if necessary ######
if (test)
    enable_testing()
    message(STATUS "Enabling GoogleTest...")
    set(BUILD_GMOCK ON CACHE BOOL "Option: build gmock and gtest -- ON" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "Option: install gtest -- OFF" FORCE)
    add_subdirectory("${THIRD_PARTY_ROOT}/googletest")
endif ()

###### Custom options ######
option(USE_JEMALLOC "Whether to use jemalloc, default: ON." ON)
option(USE_FOLLY "Whether to use folly, default: ON." ON)

#######################
# Libraries
#######################

message(STATUS "USE_FOLLY: ${USE_FOLLY}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

if (USE_FOLLY)
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Findfolly.cmake" OPTIONAL)
    if (NOT FOLLY_FOUND)
        message(STATUS "folly not found, build without folly")
    else ()
        include_directories(${FOLLY_INCLUDE_DIRS})
    endif ()
endif ()

message(STATUS "USE_JEMALLOC: ${USE_JEMALLOC}")

if (USE_JEMALLOC)
    include("cmake/FindJemalloc.cmake" OPTIONAL)
    if (NOT JEMALLOC_FOUND)
        message(STATUS "jemalloc not found, build without jemalloc")
    else ()
        add_definitions(-DUSE_JEMALLOC)
        include_directories(SYSTEM ${JEMALLOC_INCLUDE_DIRS})
    endif ()
endif ()

message(STATUS "FIND_THREAD")

# find Threads
set(CMAKE_THREAD_PREFER_PTHREAD ON)
find_package(Threads REQUIRED)

# find gflags
#message(STATUS "here")
include("cmake/FindGFlags.cmake" OPTIONAL)
if (NOT GFLAGS_FOUND)
 	message(STATUS "gflags not found, build without gflags")
else ()
 	include_directories(SYSTEM ${GFLAGS_INCLUDE_DIRS})
endif ()
#find_package(gflags REQUIRED PATHS /usr/lib/x86_64-linux-gnu/cmake/gflags/)

# find glog
include("cmake/Findglog.cmake" OPTIONAL)
if (NOT GLOG_FOUND)
    message(STATUS "glog not found, build without glog")
else ()
    include_directories(SYSTEM ${GLOG_INCLUDE_DIRS})
endif ()


# Boost-ext sml
include_directories(${THIRD_PARTY_ROOT}/sml/include)

# rapidcsv
include_directories(${THIRD_PARTY_ROOT}/rapidcsv/src)

# Eigen
include_directories(${THIRD_PARTY_ROOT}/eigen/)

# yaml-cpp
include_directories(${THIRD_PARTY_ROOT}/yaml-cpp/include/)

# common
include_directories(${PROJECT_ROOT_DIR}/common)

# googletest
include(FindPackageHandleStandardArgs)
set(GOOGLETEST_ROOT_DIR /tmp/fbcode_builder_getdeps-ZhomeZfollyZbuildZfbcode_builder-root/installed/googletest-NaaBuXP49V7fpKlpsAuaRQyAvfcmNB_1i3skI9KU8pk)
set(GOOGLETEST_INCLUDE_DIR ${GOOGLETEST_ROOT_DIR}/include)
set(GOOGLETEST_LIBRARY ${GOOGLETEST_ROOT_DIR}/lib/libgtest.a)
set(GOOGLETESTMAIN_LIBRARY ${GOOGLETEST_ROOT_DIR}/lib/libgtest_main.a)
set(GOOGLEMOCK_LIBRARY ${GOOGLETEST_ROOT_DIR}/lib/libgmock.a)
set(GOOGLEMOCKMAIN_LIBRARY ${GOOGLETEST_ROOT_DIR}/lib/libgmock_main.a)
find_package_handle_standard_args(GOOGLETEST DEFAULT_MSG GOOGLETEST_INCLUDE_DIR GOOGLETEST_LIBRARY)
if(GOOGLETEST_FOUND)
    set(GOOGLETEST_INCLUDE_DIRS ${GOOGLETEST_INCLUDE_DIR})
    set(GOOGLETEST_LIBRARIES ${GOOGLETEST_LIBRARY})
    message(STATUS "Found googletest (include: ${GOOGLETEST_INCLUDE_DIRS}, library: ${GOOGLETEST_LIBRARIES})")
    mark_as_advanced(GOOGLETEST_LIBRARY_DEBUG GOOGLETEST_LIBRARY_RELEASE
        GOOGLETEST_LIBRARY GOOGLETEST_INCLUDE_DIR GOOGLETEST_ROOT_DIR)
endif()
include_directories(SYSTEM ${GOOGLETEST_INCLUDE_DIRS})

# fmt
include(FindPackageHandleStandardArgs)
set(FMT_ROOT_DIR /tmp/fbcode_builder_getdeps-ZhomeZfollyZbuildZfbcode_builder-root/installed/fmt-eVyq_Xn_6_PY_FY4WqtE46oKigF6uY-TdSkD2lHRl7M)
set(FMT_INCLUDE_DIR ${FMT_ROOT_DIR}/include)
set(FMT_LIBRARY ${FMT_ROOT_DIR}/lib/libfmt.a)
find_package_handle_standard_args(FMT DEFAULT_MSG FMT_INCLUDE_DIR FMT_LIBRARY)
if(FMT_FOUND)
    set(FMT_INCLUDE_DIRS ${FMT_INCLUDE_DIR})
    set(FMT_LIBRARIES ${FMT_LIBRARY})
    message(STATUS "Found fmt (include: ${FMT_INCLUDE_DIRS}, library: ${FMT_LIBRARIES})")
    mark_as_advanced(FMT_LIBRARY_DEBUG FMT_LIBRARY_RELEASE
        FMT_LIBRARY FMT_INCLUDE_DIR FMT_ROOT_DIR)
endif()
include_directories(SYSTEM ${FMT_INCLUDE_DIRS})

# double_conversion
include("cmake/FindDoubleConversion.cmake" OPTIONAL)
if (NOT DOUBLECONVERSION_FOUND)
    message(STATUS "double conversion not found, build without DC")
else ()
    include_directories(SYSTEM ${DOUBLECONVERSION_INCLUDE_DIRS})
endif ()


message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")

#######################
# Artifacts
#######################
add_subdirectory(minigraph)
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/minigraph
)



file(GLOB appfiles "${CMAKE_CURRENT_SOURCE_DIR}/apps/cpp/*.cpp")
foreach (appfile ${appfiles})
    message(STATUS "Found apps: ${appfile}")
    get_filename_component(app ${appfile} NAME_WE)
    add_executable("${app}_exec" ${appfile})
    target_link_libraries("${app}_exec" PUBLIC minigraph ${FMT_LIBRARIES} ${DOUBLECONVERSION_LIBRARIES} ${GFLAGS_LIBRARIES} ${GLOG_LIBRARIES})
endforeach ()


file(GLOB toolfiles "${CMAKE_CURRENT_SOURCE_DIR}/tools/*.cpp")
foreach (toolfile ${toolfiles})
#    file(GLOB appfiles "${CMAKE_CURRENT_SOURCE_DIR}/tools/*.cpp")
    message(STATUS "Found tools: ${toolfile}")
    get_filename_component(tool ${toolfile} NAME_WE)
    add_executable("${tool}_exec" ${toolfile})
    target_link_libraries("${tool}_exec" PUBLIC minigraph ${DOUBLECONVERSION_LIBRARIES} ${GFLAGS_LIBRARIES} ${GLOG_LIBRARIES} ${FMT_LIBRARIES})
endforeach ()