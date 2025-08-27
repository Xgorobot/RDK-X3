cmake_minimum_required(VERSION 3.5)
project(xgo_gesture_control)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -fPIC)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(ai_msgs REQUIRED)
find_package(xgolib REQUIRED)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(gesture_node src/gesture_node.cpp)
ament_target_dependencies(gesture_node rclcpp ai_msgs xgolib)

target_link_libraries(gesture_node xgolib::XGO)

install(TARGETS gesture_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/launch/
  FILES_MATCHING PATTERN "*.launch.py"
)

ament_export_dependencies(rclcpp ai_msgs xgolib)
ament_package()