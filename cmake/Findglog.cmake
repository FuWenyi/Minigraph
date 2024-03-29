include(FindPackageHandleStandardArgs)

set(GLOG_ROOT_DIR /usr)

set(GLOG_INCLUDE_DIR ${GLOG_ROOT_DIR}/include/glog)
set(GLOG_LIBRARY ${GLOG_ROOT_DIR}/lib/x86_64-linux-gnu/libglog.so)

find_package_handle_standard_args(GLOG DEFAULT_MSG GLOG_INCLUDE_DIR GLOG_LIBRARY)

if(GLOG_FOUND)
    set(GLOG_INCLUDE_DIRS ${GLOG_INCLUDE_DIR})
    set(GLOG_LIBRARIES ${GLOG_LIBRARY})
    message(STATUS "Found glog (include: ${GLOG_INCLUDE_DIRS}, library: ${GLOG_LIBRARIES})")
    mark_as_advanced(GLOG_LIBRARY_DEBUG GLOG_LIBRARY_RELEASE
                     GLOG_LIBRARY GLOG_INCLUDE_DIR GLOG_ROOT_DIR)   
endif()