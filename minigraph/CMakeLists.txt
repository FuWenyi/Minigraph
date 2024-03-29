cmake_minimum_required(VERSION 3.12)
project(minigraph)

message(STATUS "in minigraph")

# Directories
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_ROOT_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_ROOT_DIR}/bin)

#######################
# Libraries
#######################
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
)

message(STATUS "USE_FOLLY: ${USE_FOLLY}")

if (USE_FOLLY)
    include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/Findfolly.cmake" OPTIONAL)
    if (NOT FOLLY_FOUND)
        message(STATUS "folly not found, build without folly")
    else ()
        include_directories(${FOLLY_INCLUDE_DIRS})
    endif ()
endif ()

message(STATUS "USE_JEMALLOC: ${USE_JEMALLOC}")

# jemalloc
if (USE_JEMALLOC)
    include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/FindJemalloc.cmake" OPTIONAL)
    if (NOT JEMALLOC_FOUND)
        message(STATUS "jemalloc not found, build without jemalloc")
    else ()
        add_definitions(-DUSE_JEMALLOC)
        include_directories(SYSTEM ${JEMALLOC_INCLUDE_DIRS})
    endif ()
endif ()

# yaml-cpp
include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/Findyaml-cpp.cmake" OPTIONAL)
include_directories(${THIRD_PARTY_ROOT}/yaml-cpp/include)
if (NOT YAML_CPP_FOUND)
    message(STATUS "yaml-cpp not found, build without yaml-cpp")
endif ()

# CUDA
find_package(CUDA)
if (NOT CUDA_FOUND)
    message(STATUS "CUDA Not Found. Project will not be build.")
endif ()

# find Threads
set(CMAKE_THREAD_PREFER_PTHREAD ON)
find_package(Threads REQUIRED)

# Boost-ext sml
include_directories(${THIRD_PARTY_ROOT}/sml/include)

# rapidcsv
include_directories(${THIRD_PARTY_ROOT}/rapidcsv/src)


#######################
# Children CMakeLists
#######################


#######################
# Artifacts
#######################

file(GLOB MINIGRAPH_SOURCES
        "*.cpp"
        "executors/*.cpp"
        "2d_pie/*.cpp"
        "graphs/*.cpp"
        "portability/*.cpp"
        "state_machine/*.cpp"
        "utility/*.cpp"
        "message_manager/*.cpp"
        )


#######################
# Tests
#######################
if (test)
    add_subdirectory("tests")
endif ()


#######################
# Generate libminigraph
#######################
add_library(minigraph ${MINIGRAPH_SOURCES})

message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "GLOG_INCLUDE_DIRS ${GLOG_INCLUDE_DIRS}")
message(STATUS "GFLAGS_INCLUDE_DIRS ${GFLAGS_INCLUDE_DIRS}")

target_link_libraries(minigraph
        PUBLIC 
        ${FOLLY_LIBRARIES}
        ${FMT_LIBRARIES}
        ${GLOG_LIBRARIES}
        ${GFLAGS_LIBRARIES}
        ${DOUBLECONVERSION_LIBRARIES}
        ${YAML_CPP_LIBRARIES}
        ${JEMALLOC_LIBRARIES}
        ${CMAKE_DL_LIBS}
	)
