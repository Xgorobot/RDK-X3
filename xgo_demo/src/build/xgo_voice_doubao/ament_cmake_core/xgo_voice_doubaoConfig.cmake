# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_xgo_voice_doubao_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED xgo_voice_doubao_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(xgo_voice_doubao_FOUND FALSE)
  elseif(NOT xgo_voice_doubao_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(xgo_voice_doubao_FOUND FALSE)
  endif()
  return()
endif()
set(_xgo_voice_doubao_CONFIG_INCLUDED TRUE)

# output package information
if(NOT xgo_voice_doubao_FIND_QUIETLY)
  message(STATUS "Found xgo_voice_doubao: 0.0.1 (${xgo_voice_doubao_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'xgo_voice_doubao' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${xgo_voice_doubao_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(xgo_voice_doubao_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${xgo_voice_doubao_DIR}/${_extra}")
endforeach()
