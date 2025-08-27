#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "xgoscreen::xgoscreen" for configuration ""
set_property(TARGET xgoscreen::xgoscreen APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(xgoscreen::xgoscreen PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libxgoscreen.so"
  IMPORTED_SONAME_NOCONFIG "libxgoscreen.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS xgoscreen::xgoscreen )
list(APPEND _IMPORT_CHECK_FILES_FOR_xgoscreen::xgoscreen "${_IMPORT_PREFIX}/lib/libxgoscreen.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
