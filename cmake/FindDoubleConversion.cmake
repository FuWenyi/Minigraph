include(FindPackageHandleStandardArgs)

set(DOUBLECONVERSION_ROOT_DIR /usr/include/double-conversion)


# We are testing only a couple of files in the include directories
set(DOUBLECONVERSION_INCLUDE_DIR ${DOUBLECONVERSION_ROOT_DIR})
set(DOUBLECONVERSION_LIBRARY ${DOUBLECONVERSION_ROOT_DIR}/../../lib/x86_64-linux-gnu/libdouble-conversion.a)

find_package_handle_standard_args(DOUBLECONVERSION DEFAULT_MSG DOUBLECONVERSION_INCLUDE_DIR DOUBLECONVERSION_LIBRARY)


if(DOUBLECONVERSION_FOUND)
    set(DOUBLECONVERSION_INCLUDE_DIRS ${DOUBLECONVERSION_INCLUDE_DIR})
    set(DOUBLECONVERSION_LIBRARIES ${DOUBLECONVERSION_LIBRARY})
    message(STATUS "Found double_conversion (include: ${DOUBLECONVERSION_INCLUDE_DIRS}, library: ${DOUBLECONVERSION_LIBRARIES})")
    mark_as_advanced(DOUBLECONVERSION_LIBRARY_DEBUG DOUBLECONVERSION_LIBRARY_RELEASE
        DOUBLECONVERSION_LIBRARY DOUBLECONVERSION_INCLUDE_DIR DOUBLECONVERSION_ROOT_DIR)
endif()