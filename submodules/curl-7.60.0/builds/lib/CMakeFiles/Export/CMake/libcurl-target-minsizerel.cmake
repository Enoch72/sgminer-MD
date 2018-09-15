#----------------------------------------------------------------
# Generated CMake target import file for configuration "MinSizeRel".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "CURL::libcurl" for configuration "MinSizeRel"
set_property(TARGET CURL::libcurl APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
set_target_properties(CURL::libcurl PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_MINSIZEREL "C;RC"
  IMPORTED_LOCATION_MINSIZEREL "${_IMPORT_PREFIX}/lib/libcurl.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS CURL::libcurl )
list(APPEND _IMPORT_CHECK_FILES_FOR_CURL::libcurl "${_IMPORT_PREFIX}/lib/libcurl.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
