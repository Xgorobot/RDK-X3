cmake_minimum_required(VERSION 3.8)
project(xgo_lidar_display)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(xgolib REQUIRED)
find_package(xgoscreen REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)

# Enable Qt5 MOC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Add executable
add_executable(lidar_display_node src/lidar_display_node.cpp)

# Specify libraries to link a library or executable target against
target_link_libraries(lidar_display_node
  ${rclcpp_LIBRARIES}
  ${sensor_msgs_LIBRARIES}
  ${std_msgs_LIBRARIES}
  ${geometry_msgs_LIBRARIES}
  ${xgolib_LIBRARIES}
  ${xgoscreen_LIBRARIES}
)

# Include directories
target_include_directories(lidar_display_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  ${rclcpp_INCLUDE_DIRS}
  ${sensor_msgs_INCLUDE_DIRS}
  ${std_msgs_INCLUDE_DIRS}
  ${geometry_msgs_INCLUDE_DIRS}
  ${xgolib_INCLUDE_DIRS}
  ${xgoscreen_INCLUDE_DIRS}
)

# Specify dependencies
ament_target_dependencies(lidar_display_node
  rclcpp
  sensor_msgs
  std_msgs
  geometry_msgs
  xgolib
  xgoscreen
)

# Link Qt5 libraries
target_link_libraries(lidar_display_node
  Qt5::Core
  Qt5::Widgets
  Qt5::Gui
)

# Install targets
install(TARGETS
  lidar_display_node
  DESTINATION lib/${PROJECT_NAME}
)

# Install launch files
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()