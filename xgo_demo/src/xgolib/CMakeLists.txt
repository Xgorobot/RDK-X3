cmake_minimum_required(VERSION 3.5)
project(xgolib)

# Use C++11 (aligned with upstream XGOLIB)
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 11)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)

# Build XGO library from vendored sources in this package
add_library(XGO STATIC
  src/XGO.cpp
)

# Provide a namespaced alias target for consumers
add_library(xgolib::XGO ALIAS XGO)

# Public headers live in include/
target_include_directories(XGO PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

if(WIN32)
  target_compile_definitions(XGO PRIVATE _WIN32)
else()
  find_package(Threads REQUIRED)
  target_link_libraries(XGO PUBLIC Threads::Threads)
endif()

# Export target to consumers as an ament package
ament_export_targets(export_XGO HAS_LIBRARY_TARGET)
ament_export_include_directories(include)
ament_export_libraries(XGO)
if(NOT WIN32)
  ament_export_dependencies(Threads)
endif()

# Install library and headers
install(TARGETS XGO
  EXPORT export_XGO
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(FILES include/XGO.h
  DESTINATION include
)

ament_package()
