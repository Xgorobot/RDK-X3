# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_xgolib_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED xgolib_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(xgolib_FOUND FALSE)
  elseif(NOT xgolib_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(xgolib_FOUND FALSE)
  endif()
  return()
endif()
set(_xgolib_CONFIG_INCLUDED TRUE)

# output package information
if(NOT xgolib_FIND_QUIETLY)
  message(STATUS "Found xgolib: 1.0.0 (${xgolib_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'xgolib' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${xgolib_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(xgolib_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "ament_cmake_export_targets-extras.cmake;ament_cmake_export_include_directories-extras.cmake;ament_cmake_export_libraries-extras.cmake;ament_cmake_export_dependencies-extras.cmake")
foreach(_extra ${_extras})
  include("${xgolib_DIR}/${_extra}")
endforeach()
