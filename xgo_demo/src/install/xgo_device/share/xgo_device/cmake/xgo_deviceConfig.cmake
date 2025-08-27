# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_xgo_device_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED xgo_device_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(xgo_device_FOUND FALSE)
  elseif(NOT xgo_device_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(xgo_device_FOUND FALSE)
  endif()
  return()
endif()
set(_xgo_device_CONFIG_INCLUDED TRUE)

# output package information
if(NOT xgo_device_FIND_QUIETLY)
  message(STATUS "Found xgo_device: 0.0.0 (${xgo_device_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'xgo_device' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${xgo_device_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(xgo_device_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${xgo_device_DIR}/${_extra}")
endforeach()
