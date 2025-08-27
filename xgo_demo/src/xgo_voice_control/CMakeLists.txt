cmake_minimum_required(VERSION 3.5)
project(xgo_voice_control)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(audio_msg REQUIRED)
find_package(xgolib REQUIRED)
find_package(xgoscreen REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui)

include_directories(
  include
  ${CMAKE_CURRENT_SOURCE_DIR}/../xgolib/include
)

add_executable(voice_control_node
  src/voice_control_node.cpp
)

ament_target_dependencies(voice_control_node rclcpp std_msgs audio_msg xgolib xgoscreen)

target_link_libraries(voice_control_node
  xgolib::XGO
  xgoscreen::xgoscreen
  Qt5::Widgets Qt5::Core Qt5::Gui
)

install(TARGETS voice_control_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY resource
  DESTINATION share/${PROJECT_NAME}
)

ament_package()