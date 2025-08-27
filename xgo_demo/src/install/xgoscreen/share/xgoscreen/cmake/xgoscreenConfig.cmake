# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_xgoscreen_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED xgoscreen_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(xgoscreen_FOUND FALSE)
  elseif(NOT xgoscreen_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(xgoscreen_FOUND FALSE)
  endif()
  return()
endif()
set(_xgoscreen_CONFIG_INCLUDED TRUE)

# output package information
if(NOT xgoscreen_FIND_QUIETLY)
  message(STATUS "Found xgoscreen: 0.0.1 (${xgoscreen_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'xgoscreen' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${xgoscreen_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(xgoscreen_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "ament_cmake_export_include_directories-extras.cmake;ament_cmake_export_targets-extras.cmake;ament_cmake_export_dependencies-extras.cmake")
foreach(_extra ${_extras})
  include("${xgoscreen_DIR}/${_extra}")
endforeach()
