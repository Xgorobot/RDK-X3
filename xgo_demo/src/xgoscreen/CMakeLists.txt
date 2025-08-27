cmake_minimum_required(VERSION 3.5)
project(xgoscreen)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

# Compilation flag -fPIC is necessary. Otherwise, you'll receive error "You must build your code with position independent code if Qt was built with -reduce-relocations."
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -fPIC)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Include directories
include_directories(
  include
  ${CMAKE_CURRENT_BINARY_DIR}
)

# Find all header files
file(GLOB_RECURSE PRO_INCLUDE_DIR RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/*.h)
file(GLOB_RECURSE PRO_SOURCES_DIR RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS src/*.cpp)

# Qt processing
qt5_wrap_cpp(QT_MOC_HPP ${PRO_INCLUDE_DIR})

# Create library
add_library(${PROJECT_NAME} SHARED
  ${PRO_SOURCES_DIR}
  ${PRO_INCLUDE_DIR}
  ${QT_MOC_HPP}
)

# Provide a namespaced alias for downstream usage
add_library(xgoscreen::xgoscreen ALIAS ${PROJECT_NAME})

# Ensure XgoScreenManager is included (in case glob patterns miss it)
target_sources(${PROJECT_NAME} PRIVATE
  src/XgoScreenManager.cpp
)

# Dependencies
ament_target_dependencies(${PROJECT_NAME}
  rclcpp
  std_msgs
  sensor_msgs
)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME}
  Qt5::Widgets
  Qt5::Core
)

# Public include directories for the target (propagate to consumers)
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
# Export include directory to downstream consumers
ament_export_include_directories(include)

# Install library
install(TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# Install headers
install(DIRECTORY include/
  DESTINATION include
)

# Export dependencies (Qt is internal, don't export to downstream)
ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)
ament_export_dependencies(rclcpp std_msgs sensor_msgs)

ament_package()