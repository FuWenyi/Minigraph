include(FindPackageHandleStandardArgs)

set(GFLAGS_ROOT_DIR /usr)
set(GFLAGS_INCLUDE_DIR ${GFLAGS_ROOT_DIR}/include/gflags)
set(GFLAGS_LIBRARY ${GFLAGS_ROOT_DIR}/lib/x86_64-linux-gnu/libgflags.so.2.2.2)

find_package_handle_standard_args(GFLAGS DEFAULT_MSG GFLAGS_INCLUDE_DIR GFLAGS_LIBRARY)

if(GFLAGS_FOUND)
    set(GFLAGS_INCLUDE_DIRS ${GFLAGS_INCLUDE_DIR})
    set(GFLAGS_LIBRARIES ${GFLAGS_LIBRARY})
    message(STATUS "Found gflags (include: ${GFLAGS_INCLUDE_DIRS}, library: ${GFLAGS_LIBRARIES})")
    mark_as_advanced(GFLAGS_LIBRARY_DEBUG GFLAGS_LIBRARY_RELEASE
                     GFLAGS_LIBRARY GFLAGS_INCLUDE_DIR GFLAGS_ROOT_DIR)   
endif()
