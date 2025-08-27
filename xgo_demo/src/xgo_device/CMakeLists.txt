cmake_minimum_required(VERSION 3.5)
project(xgo_device)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(xgolib REQUIRED)
find_package(xgoscreen REQUIRED)
find_package(Qt5 COMPONENTS Widgets Core Gui Network REQUIRED)

add_executable(device_info_node src/device_info_node.cpp)
ament_target_dependencies(device_info_node rclcpp std_msgs xgolib xgoscreen)
target_link_libraries(device_info_node xgolib::XGO xgoscreen::xgoscreen Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Network)

target_include_directories(device_info_node PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  ${Qt5Widgets_INCLUDE_DIRS}
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5Gui_INCLUDE_DIRS}
  ${Qt5Network_INCLUDE_DIRS}
)

install(TARGETS device_info_node
  DESTINATION lib/${PROJECT_NAME})

ament_package()